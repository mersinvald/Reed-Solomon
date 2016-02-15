/* Author: Mike Lubinets (aka mersinvald)
 * Date: 29.12.15
 *
 * See LICENSE */

#include "gf.hpp"
#include <string.h>

#ifdef DEBUG
#include <assert.h>
#endif

namespace RS  {

namespace gf {

/* Operations over polunomials */

void poly_scale(const Poly *p, Poly *newp, uint16_t x){
    newp->length = p->length;
    for(uint16_t i = 0; i < p->length; i++){
        newp->at(i) = mul(p->at(i), x);
    }
}

#define max(a, b) ((a > b) ? (a) : (b))

void poly_add(const Poly *p, const Poly *q, Poly *newp){
    newp->length = max(p->length, q->length);
    memset(newp->ptr(), 0, newp->length * sizeof(uint8_t));

    for(uint8_t i = 0; i < p->length; i++){
        newp->at(i + newp->length - p->length) = p->at(i);
    }

    for(uint8_t i = 0; i < q->length; i++){
        newp->at(i + newp->length - q->length) ^= q->at(i);
    }
}

void poly_mul(const Poly *p, const Poly *q, Poly *newp){
    newp->length = p->length + q->length - 1;
    memset(newp->ptr(), 0, newp->length * sizeof(uint8_t));
    /* Compute the polynomial multiplication (just like the outer product of two vectors,
     * we multiply each coefficients of p with all coefficients of q) */
    for(uint8_t j = 0; j < q->length; j++){
        for(uint8_t i = 0; i < p->length; i++){
            newp->at(i+j) ^= mul(p->at(i), q->at(j)); /* == r[i + j] = gf_add(r[i+j], gf_mul(p[i], q[j])) */
        }
    }
}

void poly_div(const Poly *p, const Poly *q, Poly *newp){
    if(p->ptr() != newp->ptr()) {
        memcpy(newp->ptr(), p->ptr(), p->length*sizeof(uint8_t));
    }

    newp->length = p->length;

    uint8_t coef;

    for(int i = 0; i < (p->length-(q->length-1)); i++){
        coef = newp->at(i);
        if(coef != 0){
            for(uint8_t j = 1; j < q->length; j++){
                if(q->at(j) != 0)
                    newp->at(i+j) ^= mul(q->at(j), coef);
            }
        }
    }

    size_t sep = p->length-(q->length-1);
    memmove(newp->ptr(), newp->ptr()+sep, (newp->length-sep) * sizeof(uint8_t));
    newp->length = newp->length-sep;
}

int8_t poly_eval(const Poly *p, uint16_t x){
    uint8_t y = p->at(0);
    for(uint8_t i = 1; i < p->length; i++){
        y = mul(y, x) ^ p->at(i);
    }
    return y;
}

} /* end of gf namespace */

} /* end of RS namespace */
