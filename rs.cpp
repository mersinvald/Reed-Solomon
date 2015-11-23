#include "rs.hpp"
#include "gf.hpp"

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
    size_t gsize;
    uint8* gen_poly = generator_poly(nsym, &gsize);

    *msg_out_size = msg_in_size+gsize-1;
    size_t sep;

    return gf::poly_div(msg_in, *msg_out_size, gen_poly, gsize, msg_out_size, &sep);
}

}
