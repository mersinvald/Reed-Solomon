#include "rs.hpp"
#include "gf.hpp"
#include <assert.h>
#include <memory.h>
#include <iostream>

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
