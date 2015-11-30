#ifndef POLYHANDLER_H
#define POLYHANDLER_H
#include <stdint.h>
#include <assert.h>

/* uncomment to initialize polynomials with zeros in startup
 * can be really needed only for debugging purposes */
// #define INIT_ZERO
/* uncomment to reset polynomials to zero on Poly::reset() */
// #define RESET_ZERO

typedef uint8_t  uint8;
typedef unsigned uint;

#define ALLOC_MEMORY(poly_handler) \
    size_t _ammem_len = poly_handler.poly_count * poly_handler.poly_size + poly_handler.msg_count * poly_handler.msg_size; \
    uint8 _alloc_memory_mem[_ammem_len]; \
    poly_handler.setmem(_alloc_memory_mem, _ammem_len);

#define ALLOC_POLY(poly_handler) \
    size_t _amp_len = poly_handler.poly_count; \
    size_t _amm_len = poly_handler.msg_count; \
    Poly _alloc_memory_pmem[_amp_len]; \
    Poly _alloc_memory_mmem[_amm_len]; \
    poly_handler.setpolymem(_alloc_memory_pmem, _alloc_memory_mmem);

#define INIT_POLYHANDLER(poly_handler) \
    ALLOC_MEMORY; \
    ALLOC_POLY; \
    poly_handler.init();

#define INIT_ENCODER 0x1
#define INIT_DECODER 0x2
#define INIT_ALL     0x3

#define DEF_MSG_COUNT 3
enum MSG_ID {
    MSG_IN    = 0,
    MSG_OUT   = 1,
    MSG_COR_E = 2
};

#define DEF_POLY_COUNT 13
enum POLY_ID {
    T_POLY1,
    T_POLY2,
    GENERATOR,

    T_POLY3,
    T_POLY4,

    SYNDROMES,
    FORNEY,

    ERASURES_LOC,
    ERRORS_LOC,

    ERASURES,
    ERRORS,

    COEF_POS,
    ERR_EVAL,
};

struct Poly {
    /* @brief Append number at the end of polynomial
     * @param num - number to append
     * @return false if polynomial can't be stretched */
    bool append(uint8 num);

    /* @brief Reset polynomial (simply set len to 0) */
    void reset();

    /* @brief Init polynomial entries
     * @param id   - polynomial number in ID enumeration
     * @param len  - polynomial lenght
     * @param size - maximum pre-allocated length
     * @param ptr  - pointer to PolyHandler memory */
    void init(uint8 id, uint8 len, uint8 size, uint8 *ptr);

    uint8& operator[] (uint8 i) const;
    bool   operator== (Poly &b) const;
    bool   operator!= (Poly &b) const;
    void   operator=  (Poly &b);

    uint8 id;
    uint8 len;
    uint8 size;
    uint8 *ptr;
};

class PolyHandler {
public:
    /* @brief PolyHandler Constructor
     * @param msglen - clear message len (w/o nsym)
     * @param nsym   - desired number of ECC bytes
     * @param flags  - init flags: INIT_ENCODER | INIT_ENCODER | INIT_ALL */
    PolyHandler(uint8 msglen, uint8 nsym, int flags);

    /* these three functions are called inside INIT_POLYHANDLER macro
     * using them manually is not recommended. */
    void setmem(uint8* ptr, uint len);
    void setpolymem(Poly* polyp, Poly* msgp);
    void init();

    /* @brief Set polynomial from uint8* array
     * @param id          - polynomial id from POLY_ID enumeration
     * @param *polynomial - uint8* array
     * @param plen        - length of polynomial */
    void set_poly(uint8 id, uint8* polynomial, uint8 plen);

    /* @brief Set message from uint8* array
     * @param id          - polynomial id from MSG_ID enumeration
     * @param *polynomial - uint8* array
     * @param plen        - length of polynomial */
    void set_msg (uint8 id, uint8 *polynomial, uint8 plen);

    /* @brief get polynomial by id */
    Poly& poly (uint8 id) const;

    /* @brief get message by id */
    Poly& msg  (uint8 id) const;

    uint8 nsym;
    uint8 poly_count;
    uint8 poly_size;
    uint8 msg_count;
    uint8 msg_size;
private:
    Poly* msg_arr;
    Poly* poly_arr;
    uint8* memory;

    uint memory_len;
    uint poly_mem_offset;
};


namespace test {
    void poly_handler_test();
    bool init_poly_test(PolyHandler &PH);
    bool set_poly_test(PolyHandler &PH);
    bool get_by_id_test(PolyHandler& PH);
    bool stretch_poly_test(PolyHandler& PH);
}

#endif // POLYHANDLER_H
