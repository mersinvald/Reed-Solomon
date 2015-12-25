#include "gf.hpp"
#include <string.h>

#ifdef DEBUG
#include <assert.h>
#endif

namespace RS  {

namespace gf {

/* Операции над полиномами */

void poly_scale(const Poly &p, Poly &newp, int x){
    newp.length = p.length;
    for(uint i = 0; i < p.length; i++){
        newp[i] = mul(p[i], x);
    }
}

#define max(a, b) ((a > b) ? (a) : (b))

void poly_add(const Poly &p, const Poly &q, Poly &newp){
    newp.length = max(p.length, q.length);
    memset(newp.ptr(), 0, newp.length * sizeof(uint8));

    for(uint i = 0; i < p.length; i++){
        newp[i + newp.length - p.length] = p[i];
    }

    for(uint i = 0; i < q.length; i++){
        newp[i + newp.length - q.length] ^= q[i];
    }
}

void poly_mul(const Poly &p, const Poly &q, Poly &newp){
    newp.length = p.length + q.length - 1;
    memset(newp.ptr(), 0, newp.length * sizeof(uint8));
    /* Compute the polynomial multiplication (just like the outer product of two vectors,
     * we multiply each coefficients of p with all coefficients of q) */
    for(uint j = 0; j < q.length; j++){
        for(uint i = 0; i < p.length; i++){
            newp[i+j] ^= mul(p[i], q[j]); /* == r[i + j] = gf_add(r[i+j], gf_mul(p[i], q[j])) */
        }
    }
}

void poly_div(const Poly &p, const Poly &q, Poly &newp){
    if(p.ptr() != newp.ptr())
        memcpy(newp.ptr(), p.ptr(), p.length*sizeof(uint8));

    newp.length = p.length;

    uint8 coef;

    for(int i = 0; i < (p.length-(q.length-1)); i++){
        coef = newp[i];
        if(coef != 0){
            for(uint j = 1; j < q.length; j++){
                if(q[j] != 0)
                    newp[i+j] ^= mul(q[j], coef);
            }
        }
    }

    size_t sep = p.length-(q.length-1);
    memmove(newp.ptr(), newp.ptr()+sep, (newp.length-sep) * sizeof(uint8));
    newp.length = newp.length-sep;
}

int poly_eval(const Poly &p, int x){
    uint8 y = p[0];
    for(uint i = 1; i < p.length; i++){
        y = mul(y, x) ^ p[i];
    }
    return y;
}

} /* end of gf namespace */

} /* end of RS namespace */
