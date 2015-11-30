#include "polyhandler.h"
#include <memory.h>

bool Poly::append(uint8 num){
    if((len+1)*sizeof(uint8) > size) return false;
    ptr[len++] = num;
    return true;
}

uint8& Poly::operator [] (uint8 i) const{
    return ptr[i];
}

bool Poly::operator == (Poly &b) const {
    if(this->len != b.len) return false;
    return (memcmp(this->ptr, b.ptr, b.len * sizeof(uint8)) == 0) ? true : false;
}

bool Poly::operator != (Poly &b) const {
    if(this->len != b.len) return true;
    return (memcmp(this->ptr, b.ptr, b.len * sizeof(uint8)) == 0) ? false : true;
}

void Poly::operator = (Poly &b) {
    memcpy(this->ptr, b.ptr, b.len);
    this->len = b.len;
}

void Poly::init(uint8 id, uint8 len, uint8 size, uint8 *ptr) {
    this->id  = id;
    this->len = len;
    this->size = size;
    this->ptr  = ptr;
#ifdef INIT_ZERO
    memset(this->ptr, 0, this->size);
#endif
}

void Poly::reset() {
    this->len = 0;
#ifdef RESET_ZERO
    memset(poly_arr[id].ptr, 0, poly_size);
#endif
}


PolyHandler::PolyHandler(uint8 msgsize, uint8 nsym, int flags) : nsym(nsym), poly_count(0), msg_count(0)
{
    if(flags & INIT_ENCODER) {
        msg_count = 2;
        poly_count = 3;
    }
    if(flags & INIT_DECODER) {
        msg_count = DEF_MSG_COUNT;
        poly_count = DEF_POLY_COUNT;
    }
    this->msg_size = msgsize+nsym;
    this->poly_size = nsym+2 + nsym/2;
}

void PolyHandler::setmem(uint8 *ptr, uint len) {
    memory = ptr;
    memory_len = len;
}

void PolyHandler::setpolymem(Poly *polyp, Poly *msgp) {
    poly_arr = polyp;
    msg_arr  = msgp;
}

void PolyHandler::init() {
    memset(memory, 0, memory_len * sizeof(uint8));
    memset(poly_arr, 0, poly_count * sizeof(Poly));
    memset(msg_arr, 0, msg_count * sizeof(Poly));

    uint offset = 0;
    for(uint i = 0; i < msg_count; i++){
        Poly &p = msg_arr[i];
        p.init(i, 0, msg_size, memory + offset);
        offset += msg_size;
    }

    this->poly_mem_offset = offset;

    for(uint j = 0; j < poly_count; j++){
        Poly &p = poly_arr[j];
        p.init(j, 0, poly_size, memory + offset);
        offset += poly_size;
    }
}

void PolyHandler::set_poly(uint8 id, uint8 *polynomial, uint8 plen){
    assert(plen <= poly_size);
    Poly &p = poly_arr[id];
    memcpy(p.ptr, polynomial, plen*sizeof(uint8));
    poly_arr[id].len = plen;
}

void PolyHandler::set_msg(uint8 id, uint8 *polynomial, uint8 plen){
    assert(plen <= msg_size);
    Poly &p = msg_arr[id];
    memcpy(p.ptr, polynomial, plen*sizeof(uint8));
    msg_arr[id].len = plen;
}

Poly& PolyHandler::poly(uint8 id) const {
    assert(id <= poly_count);
    return poly_arr[id];
}

Poly& PolyHandler::msg(uint8 id) const {
    assert(id <= msg_count);
    return msg_arr[id];
}
