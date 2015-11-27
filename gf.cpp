#include "gf.hpp"
#include <stdexcept>
#include <memory.h>
#include <iostream>

namespace gf {

u_int8_t exp[512];
u_int8_t log[256];

void init_tables(int prim){
    memset(exp, 0, 512 * sizeof(u_int8_t));
    memset(log, 0, 256 * sizeof(u_int8_t));

    /* for each value in GF(2^8) we well precompute  the logarithm and anti-logarithm(exponential) of this value */
    int x = 1;
    for(int i = 0; i < 255; i++){
        exp[i] = x;
        log[x] = i;
        x = mult_noLUT_RPM(x, 2, prim);
    }

    for(int i = 255; i < 512; i++){
        exp[i] = exp[i-255];
    }
}

/* Galua Field ariphmetics */

uint8 add(uint8 x, uint8 y){
    return x^y;
}

uint8 sub(uint8 x, uint8 y){
    return x^y;
}

int mult_noLUT(uint8 x, uint8 y, int prim){
    int result = _inner::cl_mult(x, y);
    /* do a modular reduction (ie, remainder from the division)
     * with an irreducible primitive polynomial so that it stays inside GF bounds */
    if(prim > 0) {
        result = _inner::cl_div(result, prim);
    }

    return result;
}

uint8 mult_noLUT_RPM(int x, int y, int prim, int field_charac_full, bool carryless){
    uint8 r = 0;
    /* while y is not 0 */
    while(y){
        if(y & 0x1)
            r = (carryless) ? r^x : r+x;
        y >>= 1;
        x <<= 1;
        if(prim > 0 && x & field_charac_full)
            x ^= prim;
    }
    return r;
}

uint8 mul(ushort x, ushort y) {
    if(x == 0 || y == 0) return 0;
    return exp[log[x] + log[y]];
}

uint8 div(uint8 x, uint8 y) {
    if(y == 0) throw std::overflow_error("Divide by 0 exception");
    if(x == 0) return 0;
    return exp[(log[x] + 255 - log[y]) % 255];
}

uint8 pow(uint8 x, int power) {
    int i = (log[x] * power) % 255;
    if(i < 0) i = i + 255;
    return exp[i];
}

uint8 inverse(uint8 x) {
    return exp[255 - log[x]]; /* == div(1, x); */
}

/* polynomials */

uint8* poly_scale(uint8 *p, size_t psize, int x, size_t* newsize){
    uint8* newp = new uint8[psize];
    *newsize = psize;
    for(uint i = 0; i < psize; i++){
        newp[i] = mul(p[i], x);
    }
    return newp;
}

uint8* poly_add(uint8 *p, size_t psize, uint8 *q, size_t qsize, size_t* newsize){
    uint newps = std::max(psize, qsize);
    uint8* newp = new uint8[newps];
    memset(newp, 0, newps * sizeof(uint8));
    for(uint i = 0; i < psize; i++){
        newp[i + newps - psize] = p[i];
    }
    for(uint i = 0; i < qsize; i++){
        newp[i + newps - qsize] ^= q[i];
    }
    *newsize = newps;
    return newp;
}

uint8* poly_mul(uint8 *p, size_t psize, uint8 *q, size_t qsize, size_t *newsize){
    /* pre-allocate resulting array */
    uint8* newp = new uint8[psize + qsize - 1];
    memset(newp, 0, psize * sizeof(uint8));
    /* Compute the polynomial multiplication (just like the outer product of two vectors,
     * we multiply each coefficients of p with all coefficients of q) */
    for(uint j = 0; j < qsize; j++){
        for(uint i = 0; i < psize; i++){
            newp[i+j] ^= mul(p[i], q[j]); /* == r[i + j] = gf_add(r[i+j], gf_mul(p[i], q[j])) */
        }
    }
    *newsize = psize + qsize - 1;
    return newp;
}

uint8* poly_div(uint8 *p, size_t psize, uint8 *q, size_t qsize, size_t *newsize){
    size_t outsize = psize;
    uint8* out = new uint8[psize];
    memcpy(out, p, psize*sizeof(uint8));

    uint8 coef;

    for(uint i = 0; i < psize-(qsize-1); i++){
        coef = out[i];
        if(coef != 0){
            for(uint j = 1; j < qsize; j++){
                if(q[j] != 0)
                    out[i+j] ^= mul(q[j], coef);
            }
        }
    }

    size_t sep = psize-(qsize-1);
    memmove(out, out+sep, (outsize-sep) * sizeof(uint8));
    realloc(out, outsize-(outsize+sep));
    *newsize = (outsize-sep);
    return out;
}

int poly_eval(uint8 *p, size_t psize, int x){
    uint8 y = p[0];
    for(uint i = 1; i < psize; i++){
        y = mul(y, x) ^ p[i];
    }
    return y;
}

} /* end of gf namespace */

namespace gf {
namespace _inner {
uint bit_len(int n) {
    uint bits = 0;
    while(n >> bits) bits++;
    return bits;
}

int cl_mult(int x, int y) {
    ushort z = 0;
    ushort i = 0;
    while((y >> i) > 0) {
        if(y & (1 << i)){
            z ^= x << i;
        }
        i++;
    }
    return z;
}

int cl_div(int dividend, int divisor) {
    /* compute the position of most significant integer for both dividend and divisor */
    int ddl = bit_len(dividend);
    int drl = bit_len(divisor);

    /* if dividend is smaller then divisor, exit */
    if(ddl < drl) return dividend;
    for(int i = ddl-drl; i >= 0; i--){
        /* Check that the dividend is divisible */
        if(dividend & (1 << (i+drl-1))){
            /* then shift the divisor to align the most significant bits and XOR (carry-less subtraction) */
            dividend ^= divisor << i;
        }
    }
    return dividend;
}
} /* end of _inner namespace */

}
