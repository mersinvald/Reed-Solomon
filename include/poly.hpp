/* Author: Mike Lubinets (aka mersinvald)
 * Date: 29.12.15
 *
 * See LICENSE */

#ifndef POLY_H
#define POLY_H
#include <stdint.h>
#include <string.h>

#ifdef DEBUG
#include <assert.h>
#endif

namespace RS {

 
#ifdef __CC_ARM
#define nullptr NULL
#endif
    
struct Poly {
    Poly()
        : length(0), _memory(nullptr) {}

    Poly(uint8_t id, uint16_t offset, uint8_t size) \
        : length(0), _id(id), _size(size), _offset(offset), _memory(nullptr) {}

    /* @brief Append number at the end of polynomial
     * @param num - number to append
     * @return false if polynomial can't be stretched */
    inline bool Append(uint8_t num) {
        #ifdef DEBUG
        assert(length+1 < _size);
        #endif
        ptr()[length++] = num;
        return true;
    }

    inline bool Append(const uint8_t* array, size_t size) {
        #ifdef DEBUG
        assert(length+size < _size);
        #endif
        memcpy(ptr() + length, array, size);
        length += size;
        return true;
    }

    /* @brief Polynomial initialization */
    inline void Init(uint8_t id, uint16_t offset, uint8_t size, uintptr_t memory_ptr) {
        this->_id     = id;
        this->_offset = offset;
        this->_size   = size;
        this->length  = 0;
        this->_memory  = (uint8_t**) memory_ptr;
    }

    /* @brief Polynomial memory zeroing */
    inline void Reset() {
        memset(ptr(), 0, this->_size);
    }

    /* @brief Copy polynomial to memory
     * @param src    - source byte-sequence
     * @param size   - size of polynomial
     * @param offset - write offset */
    inline void Set(const uint8_t* src, uint8_t len, uint16_t offset = 0) {
        #ifdef DEBUG
        assert(src && _size <= this->_size-offset);
        #endif
        memcpy(ptr()+offset, src, len * sizeof(uint8_t));
        length = len;
    }

    #define max(a, b) ((a > b) ? (a) : (b))

    inline bool operator== (Poly &b) const {
        return memcmp(this->ptr(), b.ptr(), max(this->length, b.length)) == 0;
    }

    inline bool operator!= (Poly &b) const {
        return memcmp(this->ptr(), b.ptr(), max(this->length, b.length));
    }

    inline void operator=  (const Poly &b) {
        length = max(length, b.length);
        Set(b.ptr(), length);
    }

    inline uint8_t& operator[] (uint8_t i) const {
        #ifdef DEBUG
        assert(i < _size);
        #endif
        return ptr()[i];
    }

    inline uint8_t id() const {
        return _id;
    }

    inline uint8_t size() const {
        return _size;
    }

    // Returns pointer to memory of this polynomial
    inline uint8_t* ptr() const {
        #ifdef DEBUG
        assert(_memory && *_memory);
        #endif
        return (*_memory) + _offset;
    }

    uint8_t length;

protected:

    uint8_t   _id;
    uint8_t   _size;    // Size of reserved memory for this polynomial
    uint16_t  _offset;  // Offset in memory
    uint8_t** _memory;  // Pointer to pointer to memory
};


}

#endif // POLY_H
