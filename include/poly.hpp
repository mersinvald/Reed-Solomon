#ifndef POLY_H
#define POLY_H
#include <stdint.h>
#include <string.h>

#ifdef DEBUG
#include <assert.h>
#endif

namespace RS {

typedef uint8_t  uint8;
typedef uint32_t uint;
 
#ifdef __CC_ARM
#define nullptr NULL
#endif
    
struct Poly {
    Poly()
        : length(0), memory(nullptr) {}

    Poly(uint8 id, uint offset, uint8 size) \
        : length(0), _id(id), _size(size), _offset(offset), memory(nullptr) {}

    /* @brief Append number at the end of polynomial
     * @param num - number to append
     * @return false if polynomial can't be stretched */
    inline bool Append(uint8 num) {
        #ifdef DEBUG
        assert(length+1 < _size);
        #endif
        ptr()[length++] = num;
        return true;
    }

    /* @brief Инициализация значений полинома */
    inline void Init(uint8 id, uint offset, uint8 size, uint8** memory) {
        this->_id     = id;
        this->_offset = offset;
        this->_size   = size;
        this->length  = 0;
        this->memory  = memory;
    }

    /* @brief Обнуление памяти полинома */
    inline void Reset() {
        memset(ptr(), 0, this->_size);
    }

    /* @brief Скопировать в массив полинома из внешнего буффера
     * @param src    - указатель на источник
     * @param size   - размер источника
     * @param offset - смещение в памяти полинома */
    inline void Set(uint8* src, uint8 len, uint offset = 0) {
        #ifdef DEBUG
        assert(src && _size <= this->_size-offset);
        #endif
        memcpy(ptr()+offset, src, len * sizeof(uint8));
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

    inline uint8& operator[] (uint8 i) const {
        #ifdef DEBUG
        assert(i < _size);
        #endif
        return ptr()[i];
    }

    inline uint8 id() const {
        return _id;
    }

    inline uint8 size() const {
        return _size;
    }

    // Указатель на область памяти на стеке
    inline uint8* ptr() const {
        #ifdef DEBUG
        assert(memory && *memory);
        #endif
        return (*memory) + _offset;
    }

    uint8 length;

protected:

    uint8   _id;
    uint8   _size;    // Максимальный размер, отведенный для полинома в общей памяти
    uint    _offset;  // Смещение в общей памяти
    uint8** memory; // Указатель на указатель на общую память для полиномов на стеке

};


}

#endif // POLY_H
