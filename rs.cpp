#include "rs.hpp"
#include "gf.hpp"
#include <assert.h>
#include <memory.h>
#include <iostream>

/* ################
 * ### ENCODING ###
 * ################ */

namespace RS {
void generator_poly(int nsym, Poly &generator, PolyHandler &PH) {
    generator[0] = 1;
    generator.len = 1;

    Poly &mulp = PH.poly(T_POLY1);
    Poly &temp = PH.poly(T_POLY2);
    mulp.len = 2;

    for(int i = 0; i < nsym; i++){
        mulp[0] = 1;
        mulp[1] = gf::pow(2, i);

        gf::poly_mul(generator, mulp, temp);

        generator.len = temp.len;
        memcpy(generator.ptr, temp.ptr, temp.len * sizeof(uint8));
    }
}

void encode_msg(void *msg_in, size_t msg_in_size, void *msg_out, size_t msg_out_size, int nsym) {
    uint8 *msg_in_ptr  = (uint8*) msg_in;
    uint8 *msg_out_ptr = (uint8*) msg_out;
    uint16_t msg_in_len   = msg_in_size / sizeof(uint8);
    uint16_t msg_out_len  = msg_out_size / sizeof(uint8);

    assert((msg_in_len + nsym) < 256);
    assert(msg_out_len >= (msg_in_len + nsym));

    PolyHandler PH(msg_in_len, nsym, INIT_ENCODER);
    ALLOC_MEMORY(PH);
    ALLOC_POLY(PH);
    PH.init();

    Poly &generator = PH.poly(GENERATOR);
    generator.reset();
    generator_poly(nsym, generator, PH);

    Poly& msg_in_poly  = PH.msg(MSG_IN);
    Poly& msg_out_poly = PH.msg(MSG_OUT);

    msg_in_poly.reset();
    msg_out_poly.reset();

    PH.set_msg(MSG_IN, msg_in_ptr, msg_in_len);

    msg_out_poly = msg_in_poly;
    msg_out_poly.len = msg_in_poly.len + generator.len - 1;

    uint8 coef;
    for(uint i = 0; i < msg_in_len; i++){
        coef = msg_out_poly[i];
        if(coef != 0){
            for(uint j = 1; j < generator.len; j++){
                msg_out_poly[i+j] ^= gf::mul(generator[j], coef);
            }
        }
    }
    memcpy(msg_out_poly.ptr, msg_in_poly.ptr, msg_in_poly.len * sizeof(uint8));
    memcpy(msg_out_ptr, msg_out_poly.ptr, msg_out_poly.len * sizeof(uint8));
}
}

/* ################
 * ### DECODING ###
 * ################ */

namespace RS {

void calc_syndromes(const Poly &msg, uint nsym, Poly &synd) {
    synd.len = nsym+1;
    memset(synd.ptr, 0, synd.len * sizeof(uint8));
    for(uint i = 1; i < nsym+1; i++){
        synd[i] = gf::poly_eval(msg, gf::pow(2, i-1));
    }
}

void find_errata_locator(const Poly &epos, Poly &errata_loc, PolyHandler &PH) {
    errata_loc.len = 1;
    errata_loc[0] = 1;

    Poly &mulp = PH.poly(T_POLY1);
    Poly &addp = PH.poly(T_POLY2);
    Poly &apol = PH.poly(T_POLY3);
    Poly &temp = PH.poly(T_POLY4);

    mulp.len = 1;
    addp.len = 2;

    for(uint i = 0; i < epos.len; i++){
        mulp[0] = 1;
        addp[0] = gf::pow(2, epos[i]);
        addp[1] = 0;

        gf::poly_add(mulp, addp, apol);
        gf::poly_mul(errata_loc, apol, temp);

        errata_loc = temp;
    }
}

void find_error_evaluator(const Poly &synd, const Poly &errata_loc, int nsym, Poly &err_eval, PolyHandler &PH) {
    Poly &mulp = PH.poly(T_POLY1);
    gf::poly_mul(synd, errata_loc, mulp);

    Poly &divisor = PH.poly(T_POLY2);
    divisor.len = nsym+2;

    memset(divisor.ptr, 0, divisor.len*sizeof(uint8));
    divisor[0] = 1;

    gf::poly_div(mulp, divisor, err_eval);
}

void correct_errata(const Poly &synd, const Poly &err_pos, const Poly &msg_in, Poly &corrected, PolyHandler &PH) {
    Poly &coef_pos = PH.poly(COEF_POS);
    coef_pos.len = err_pos.len;

    for(uint i = 0; i < err_pos.len; i++)
        coef_pos[i] = msg_in.len - 1 - err_pos[i];

    Poly &errata_loc = PH.poly(ERASURES_LOC);
    errata_loc.reset();

    /* uses T_POLY 1, 2, 3, 4 */
    find_errata_locator(coef_pos, errata_loc, PH);

    /* reversyng syndromes */
    Poly &rsynd = PH.poly(T_POLY3);
    rsynd.len = synd.len;

    for(int i = synd.len-1, j = 0; i >= 0; i--, j++) {
        rsynd[j] = synd[i];
    }

    /* getting reversed error evaluator polynomial */
    Poly &rerr_eval = PH.poly(T_POLY4);

    /* uses T_POLY 1, 2 */
    find_error_evaluator(rsynd, errata_loc, errata_loc.len-1, rerr_eval, PH);

    /* reversing it back */
    Poly &err_eval = PH.poly(ERR_EVAL);
    for(int i = rerr_eval.len-1, j = 0; i >= 0; i--, j++)
        err_eval[j] = rerr_eval[i];


    Poly &X = PH.poly(T_POLY1); /* this will store errors positions */
    X.len = 0;
    memset(X.ptr, 0, X.len * sizeof(uint8));

    int16_t l;
    for(uint i = 0; i < coef_pos.len; i++){
        l = 255 - coef_pos[i];
        X.append(gf::pow(2, -l));
    }

    /* Magnitude polynomial */
    Poly &E = PH.msg(MSG_COR_E);
    E.len = msg_in.len;
    memset(E.ptr, 0, E.len * sizeof(uint8));

    uint8 Xi_inv;

    Poly &err_loc_prime_temp = PH.poly(T_POLY2);

    uint8 err_loc_prime;
    uint8 y;

    for(uint i = 0; i < X.len; i++){
        Xi_inv = gf::inverse(X[i]);

        err_loc_prime_temp.reset();
        for(uint j = 0; j < X.len; j++){
            if(j != i){
                err_loc_prime_temp.append(gf::sub(1, gf::mul(Xi_inv, X[j])));
            }
        }

        err_loc_prime = 1;
        for(uint j = 0; j < err_loc_prime_temp.len; j++){
            err_loc_prime = gf::mul(err_loc_prime, err_loc_prime_temp[j]);
        }

        y = gf::poly_eval(rerr_eval, Xi_inv);
        y = gf::mul(gf::pow(X[i], 1), y);

        E[err_pos[i]] = gf::div(y, err_loc_prime);
    }

    gf::poly_add(msg_in, E, corrected);
}

//(uint8 *synd, size_t synd_size, int nsym, size_t *newsize, uint8 *erase_loc, size_t erase_loc_size, size_t erase_count)
bool find_error_locator(const Poly &synd, int nsym, Poly &error_loc, PolyHandler &PH, Poly *erase_loc, size_t erase_count) {
    Poly &err_loc = PH.poly(T_POLY1);
    Poly &old_loc = PH.poly(T_POLY2);
    Poly &temp    = PH.poly(T_POLY3);
    Poly &temp2   = PH.poly(T_POLY4);

    if(erase_loc != NULL) {
        err_loc = *erase_loc;
        old_loc = *erase_loc;
    } else {
        err_loc.len = 1;
        old_loc.len = 1;
        err_loc[0] = 1;
        old_loc[0] = 1;
    }

    uint synd_shift = 0;
    if(synd.len > nsym)
        synd_shift = synd.len - nsym;

    uint8 K = 0;
    uint8 delta = 0;
    uint8 index;

    for(uint i = 0; i < nsym - erase_count; i++){
        if(erase_loc != NULL)
            K = erase_count + i + synd_shift;
        else
            K = i + synd_shift;

        delta = synd[K];
        for(uint j = 1; j < err_loc.len; j++) {
            index = err_loc.len - j - 1;
            delta ^= gf::mul(err_loc[index], synd[K-j]);
        }

        old_loc.append(0);

        if(delta != 0) {
            if(old_loc.len > err_loc.len) {
                gf::poly_scale(old_loc, temp, delta);
                gf::poly_scale(err_loc, old_loc, gf::inverse(delta));
                err_loc = temp;
            }
            gf::poly_scale(old_loc, temp, delta);
            gf::poly_add(err_loc, temp, temp2);
            err_loc = temp2;
        }
    }

    uint shift = 0;
    while(err_loc.len && err_loc[shift] == 0) shift++;

    uint errs = err_loc.len - shift - 1;
    if(((errs - erase_count) * 2 + erase_count) > nsym){
        return false; /* количество ошибок больше, чем может быть исправлено! */
    }

    memcpy(error_loc.ptr, err_loc.ptr + shift, (err_loc.len - shift) * sizeof(uint8));
    error_loc.len = (err_loc.len - shift);
    return true;
}

bool find_errors(const Poly &error_loc, Poly &errors, size_t msg_in_size){
    uint8 errs = error_loc.len - 1;
    errors.len = 0;

    for(uint i = 0; i < msg_in_size; i++) {
        if(gf::poly_eval(error_loc, gf::pow(2, i)) == 0) {
            errors.append(msg_in_size - 1 - i);
        }
    }

    /* Sanity check:
     * the number of errors/errata positions found
     * should be exactly the same as the length of the errata locator polynomial */
    if(errors.len != errs)
        /* couldn't find error locations */
        return false;
    return true;
}

void forney_syndromes(const Poly &synd, const Poly &erasures_pos, Poly &forney_synd, size_t msg_in_size, PolyHandler &PH) {
    Poly &erase_pos_reversed = PH.poly(T_POLY1);
    erase_pos_reversed.reset();
    for(uint i = 0; i < erasures_pos.len; i++){
        erase_pos_reversed.append(msg_in_size - 1 - erasures_pos[i]);
    }

    PH.set_poly(forney_synd.id, synd.ptr+1, synd.len-1);

    uint8 x;
    for(uint i = 0; i < erasures_pos.len; i++) {
        x = gf::pow(2, erase_pos_reversed[i]);
        for(int j = 0; j < forney_synd.len - 1; j++){
            forney_synd[j] = gf::mul(forney_synd[j], x) ^ forney_synd[j+1];
        }
    }
}

bool correct_msg(void *msg_in, size_t msg_in_size, void *msg_out, size_t msg_out_size, int nsym, uint8 *erase_pos, size_t erase_count) {
    uint8 *msg_in_ptr = (uint8*) msg_in;
    uint8 *msg_out_ptr = (uint8*) msg_out;

    uint16_t msg_in_len = msg_in_size / sizeof(uint8);
    uint16_t msg_out_len = msg_out_size / sizeof(uint8);

    assert(msg_in_len < 255);
    assert(msg_out_len <= msg_in_len - nsym);

    PolyHandler PH(msg_in_len - nsym, nsym, INIT_DECODER);
    ALLOC_MEMORY(PH);
    ALLOC_POLY(PH);
    PH.init();

    Poly &msgin  = PH.msg(MSG_IN);
    Poly &msgout = PH.msg(MSG_OUT);
    Poly &epos = PH.poly(ERASURES);

    PH.set_msg(msgin.id, msg_in_ptr, msg_in_len);

    msgout = msgin;

    if(erase_pos == NULL) {
        epos.len = 0;
    } else {
        PH.set_poly(epos.id, erase_pos, erase_count);
        for(uint i = 0; i < epos.len; i++){
            msgin[epos[i]] = 0;
        }
    }

    /* too many errors */
    if(epos.len > nsym) return false;

    Poly &synd   = PH.poly(SYNDROMES);
    Poly &fsynd = PH.poly(FORNEY);
    Poly &eloc  = PH.poly(ERRORS_LOC);
    Poly &reloc = PH.poly(T_POLY1);
    Poly &errors = PH.poly(ERRORS);

    calc_syndromes(msgin, nsym, synd);

    bool has_errors = false;
    for(uint i = 0; i < synd.len; i++) {
        if(synd[i] != 0) {
            has_errors = true;
            break;
        }
    }

    if(!has_errors) goto return_corrected_msg;


    forney_syndromes(synd, epos, fsynd, msg_in_len, PH);
    find_error_locator(fsynd, nsym, eloc, PH, NULL, epos.len);

    /* reversing eloc for find_errors() */
    reloc.reset();
    reloc.len = eloc.len;
    for(int i = eloc.len-1, j = 0; i >= 0; i--, j++){
        reloc[j] = eloc[i];
    }

    find_errors(reloc, errors, msg_in_len);

    if(errors.len == 0) return false;

    /* adding errors to erasures */
    for(uint i = 0; i < errors.len; i++) {
        epos.append(errors[i]);
    }

    correct_errata(synd, epos, msgin, msgout, PH);

return_corrected_msg:
    msgout.len -= nsym;
    memcpy(msg_out_ptr, msgout.ptr, msgout.len * sizeof(uint8));
    return true;
}

}

