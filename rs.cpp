#include "rs.hpp"
#include "gf.hpp"
#include <assert.h>
#include <memory.h>
#include <iostream>

/* ################
 * ### ENCODING ###
 * ################ */

namespace RS {
uint8* generator_poly(int nsym, size_t* size){
    uint8* g = new uint8[1];
    uint8* temp;
    uint8 mulp[2];
    g[0] = 1;

    size_t gsize = 1;

    for(int i = 0; i < nsym; i++){
        mulp[0] = 1;
        mulp[1] = gf::pow(2, i);
        temp = gf::poly_mul(g, gsize, mulp, 2, &gsize);
        delete g;
        g = temp;
    }

    *size = gsize;

    return g;
}

uint8* encode_msg(uint8 *msg_in, size_t msg_in_size, int nsym, size_t *msg_out_size){
    assert((msg_in_size + nsym) < 256);

    size_t gens;
    uint8* genp = generator_poly(nsym, &gens);

    size_t outs = msg_in_size + gens - 1;
    uint8* out  = new uint8[outs];
    memset(out, 0, outs * sizeof(uint8));
    memcpy(out, msg_in, msg_in_size * sizeof(uint8));

    uint8 coef;
    for(uint i = 0; i < msg_in_size; i++){
        coef = out[i];
        if(coef != 0){
            for(uint j = 1; j < gens; j++){
                out[i+j] ^= gf::mul(genp[j], coef);
            }
        }
    }
    memcpy(out, msg_in, msg_in_size * sizeof(uint8));
    *msg_out_size = outs;
    return out;
}
}

/* ################
 * ### DECODING ###
 * ################ */

namespace RS {

uint8* calc_syndromes(uint8 *msg, size_t msg_size, uint nsym, size_t *synd_size){
    uint8* synd = (uint8*) calloc(nsym+1, (nsym+1) * sizeof(uint8));
    for(uint i = 1; i < nsym+1; i++){
        synd[i] = gf::poly_eval(msg, msg_size, gf::pow(2, i-1));
    }
    *synd_size = nsym+1;
    return synd;
}

bool check(uint8 *msg, size_t msg_size, int nsym){
     size_t synd_size;
    uint8* synd = calc_syndromes(msg, msg_size, nsym, &synd_size);

    for(uint i = 0; i < synd_size; i++){
        if(synd[i] != 0) return false;
    }
    return true;
}

uint8* find_errata_locator(uint8 *e_pos, size_t e_pos_size, size_t *errloc_size){
    uint8* e_loc = new uint8[1];
    e_loc[0] = 1;
    size_t eloc_size = 1;
    uint8* temp;

    uint8 mulp[1];
    uint8 addp[2];

    size_t asize;
    uint8* aptr;

    for(uint i = 0; i < e_pos_size; i++){
        mulp[0] = 1;
        addp[0] = gf::pow(2, e_pos[i]);
        addp[1] = 0;

        aptr = gf::poly_add(mulp, 1, addp, 2, &asize);
        temp = gf::poly_mul(e_loc, eloc_size, aptr, asize, &eloc_size);

        delete aptr;
        delete e_loc;

        e_loc = temp;
    }

    *errloc_size = eloc_size;
    return e_loc;
}

uint8* find_error_evaluator(uint8 *synd, size_t synd_size, uint8 *err_loc, size_t err_loc_size, int nsym, size_t *err_eval_size){
    size_t mulps;
    uint8* mulp = gf::poly_mul(synd, synd_size, err_loc, err_loc_size, &mulps);

    uint8 divisor[nsym+2];
    memset(&divisor, 0, (nsym+2)*sizeof(uint8));
    divisor[0] = 1;

    uint8* remainder =  gf::poly_div(mulp, mulps, divisor, nsym+2, err_eval_size);

    delete mulp;

    return remainder;
}

uint8* correct_errata(uint8 *msg_in, size_t msg_in_size, uint8 *synd, size_t synd_size, uint8 *err_pos, size_t epos_size, size_t *newsize){
    uint8 coef_pos[epos_size];
    for(uint i = 0; i < epos_size; i++)
        coef_pos[i] = msg_in_size - 1 - err_pos[i];

    size_t eloc_size;
    uint8 *err_loc = find_errata_locator(coef_pos, epos_size, &eloc_size);

    /* reversing synd */
    uint8 rsynd[synd_size];

    int i, j ;
    for(i = synd_size-1, j = 0; i >= 0; i--, j++) {
        rsynd[j] = synd[i];
    }

    /* getting reversed error evaluator polynomial */
    size_t eeval_size;
    uint8 *rerr_eval = find_error_evaluator(rsynd, synd_size, err_loc, eloc_size, eloc_size-1, &eeval_size);

    /* reversing it back */
    uint8 err_eval[eeval_size];
    for(int i = eeval_size-1, j = 0; i >= 0; i--, j++)
        err_eval[j] = rerr_eval[i];


    /* @TODO: use half-dynamic arrays everywrere */
    size_t Xlen = 0;
    uint8 X[epos_size]; /* this will store errors positions */
    memset(X, 0, epos_size * sizeof(uint8));
    int16_t l;
    for(uint i = 0; i < epos_size; i++){
        l = 255 - coef_pos[i];
        X[(Xlen++)] = gf::pow(2, -l);
    }

    /* Magnitude polynomial */
    size_t &Esize = msg_in_size;
    uint8 E[msg_in_size];
    memset(E, 0, Esize * sizeof(uint8));

    uint8 Xi_inv;

    size_t elp_temp_len = 0;
    size_t elp_temp_maxlen = (epos_size)*(epos_size-1);
    uint8 err_loc_prime_temp[elp_temp_maxlen];
    memset(err_loc_prime_temp, 0, (epos_size)*(epos_size-1) * sizeof(uint8));

    uint8 err_loc_prime;
    uint8 y;

    for(uint i = 0; i < Xlen; i++){
        Xi_inv = gf::inverse(X[i]);

        elp_temp_len = 0;
        for(uint j = 0; j < Xlen; j++){
            if(j != i){
                err_loc_prime_temp[elp_temp_len++] = gf::sub(1, gf::mul(Xi_inv, X[j]));
            }
        }

        err_loc_prime = 1;
        for(uint j = 0; j < elp_temp_len; j++){
            err_loc_prime = gf::mul(err_loc_prime, err_loc_prime_temp[j]);
        }

        y = gf::poly_eval(rerr_eval, eeval_size, Xi_inv);
        y = gf::mul(gf::pow(X[i], 1), y);

        E[err_pos[i]] = gf::div(y, err_loc_prime);
    }

    msg_in = gf::poly_add(msg_in, msg_in_size, E, Esize, newsize);

    return msg_in;
}

uint8* find_error_locator(uint8 *synd, size_t synd_size, int nsym, size_t *newsize, uint8 *erase_loc, size_t erase_loc_size, size_t erase_count){
    uint8 err_loc[erase_loc_size];
    uint8 old_loc[erase_loc_size];
    size_t err_loc_size;

    if(erase_loc != NULL) {
        memcpy(err_loc, erase_loc, erase_loc_size * sizeof(uint8));
        memcpy(old_loc, erase_loc, erase_loc_size * sizeof(uint8));
        err_loc_size = erase_loc_size;
    } else {
        err_loc[0] = 1;
        old_loc[0] = 1;
        err_loc_size = 1;
    }

    uint synd_shift = 0;
    if(synd_size > nsym)
        synd_shift = synd_size - nsym;

    uint8 K = 0;
    uint8 delta = 0;
    uint8 index;

    for(uint i = 0; i < nsym - erase_count; i++){
        if(erase_loc != NULL)
            K = erase_count + i + synd_shift;
        else
            K = i + synd_shift;

        delta = synd[K];
        for(uint j = 1; j < err_loc_size; j++) {
            index = err_loc_size - j;
            delta ^= gf::mul(err_loc[index], synd[K-j]);
        }
    }

}

uint8* find_errors(uint8 *err_loc, size_t eloc_size, size_t msg_in_size){

}

uint8* forney_syndromes(uint8 *synd, size_t synd_size, size_t msg_in_size){

}

uint8* correct_msg(uint8 *msg_in, size_t msg_in_size, int nsym, uint8 *erase_pos, size_t epos_size){

}

}
