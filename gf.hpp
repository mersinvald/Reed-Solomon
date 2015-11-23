#ifndef GF_H
#define GF_H
#include <stdlib.h>

typedef u_int8_t uint8;

namespace gf {

extern u_int8_t exp[512]; /* anti-log (exponential) table */
extern u_int8_t log[256]; /* log table */


/* @brief Lookup tables initialization
 * @param prim - primitibe polinominal in GF, 0x11d - common primitiva polinominal for Reed-Solomon codes
 */
void init_tables(int prim = 0x11d);

/* ################################
 * # OPERATIONS OVER GALUA FIELDS #
 * ################################ */

/* @brief Addition in Galua Fields
 * @param x - left operand
 * @param y - right operand
 * @return result of addition
 */
uint8 add(uint8 x, uint8 y);

/* ##### GF substraction ###### */
/* @brief Substraction in Galua Fields
 * @param x - left operand
 * @param y - right operand
 * @return result of substraction
 */
uint8 sub(uint8 x, uint8 y);

/* @brief Multiplication in Galua Fields without lookup table (slow)
 * @param x - left operand
 * @param y - right operand
 * @param prim - primitive polinominal of GF
 * @return result of multiplication
 */
int mult_noLUT(uint8 x, uint8 y, int prim = 0);

/* @brief Multiplication in Galua Fields w/o lookup table, Russian Peasant Multiplication algorithm
 * @param x - left operand
 * @param y - right operand
 * @param prim - primitive polinominal of GF
 * @param field_charac_full -
 * @return result of multiplication
 */
uint8 mult_noLUT_RPM(int x, int y, int prim = 0, int field_charac_full = 256, bool carryless = true);

/* @brief Multiplication in Galua Fields w/ lookup table (needs to be initialized)
 * @param x - left operand
 * @param y - right operand
 * @return result of multiplication
 */
uint8 mul(ushort x, ushort y);

/* @brief Division in Galua Fields w/ lookup table (needs to be initialized)
 * @param x - dividend
 * @param y - divisor
 * @return result of division
 */
uint8 div(uint8 x, uint8 y);

/* @brief X in power Y w/ lookup table (needs to be initialized)
 * @param x - operand
 * @param power - power
 * @return x^power
 */
uint8 pow(uint8 x, int power);

/* @brief Inversion in Galua Fields w/ lookup table (needs to be initialized)
 * @param x - number
 * @return inversion of x
 */
uint8 inverse(uint8 x);

/* ##########################
 * # POLYNOMIALS OPERATIONS #
 * ########################## */

/* @brief Multiplication polinomial by scalar
 * @param *p - polynomial pointer
 * @param psize - polynomial size
 * @param x - scalar
 * @param *newp - pointer to new polynomial
 * @param *newsize - pointer to new polunomial size
 * @return pointer to new polynomial
 */
uint8* poly_scale(uint8* p, size_t psize, int x, size_t* newsize);

/* @brief Addition of two polynomials
 * @param *p - polynomial pointer
 * @param psize - len of p array
 * @param *q - second polinomial pointer
 * @param qsize - len of q array
 * @param *newp - pointer to new polinomial
 * @param *newsize - ptr to write new polynomial len
 * @return pointer to new polunomial
 */
uint8* poly_add(uint8* p, size_t psize, uint8* q, size_t qsize, size_t* newsize);

/* @brief Multiplication of two polynomials
 * @param *p - first polynomial pointer
 * @param psize - first polinomial size
 * @param *q - second polinomial pointer
 * @param qsize - second polinomial pointer
 * @param *newp - pointer to new polynomial
 * @param *newsize - pointer to size of new polynomial
 * @return pointer to new polunomial
 */
uint8* poly_mul(uint8* p, size_t psize, uint8* q, size_t qsize, size_t* newsize);

/* @brief Multiplication of two polynomials
 * @param *p - first polynomial pointer
 * @param psize - first polinomial size
 * @param *q - second polinomial pointer
 * @param qsize - second polinomial pointer
 * @param *newp - pointer to new polynomial
 * @param *newsize - pointer to size of new polynomial
 * @param *sep_pos - pointer to position of separator between data and RS code
 * @return pointer to remainder
 */
uint8* poly_div(uint8* p, size_t psize, uint8* q, size_t qsize, size_t* newsize);

/* @brief Evaluation of polynomial in x
 * @param *p - polynomial pointer
 * @param  x - point where to evaluate
 * @return evaluation
 */
int poly_eval(uint8* p, size_t psize, int x);

namespace _inner {
/* GF multiplication sub-funcs */
/* computes the biggest 1 bit of integer */
uint bit_len(int n);
/* carry-less multiplication on integers */
int cl_mult(int x, int y);
/* carry-less long division on integers, returns the reminder */
int cl_div (int dividend, int divisor);
} /* end of gf::_inner namespace */

namespace test {
/* ##### TESTS ##### */
namespace _inner {
bool bit_len_test();
bool cl_mult_test();
bool cl_div_test();
}
/* GF ariphmetics */
bool init_tables_test();
bool mult_noLUT_test();
bool mult_noLUT_RPM_test();
bool add_test();
bool sub_test();
bool mul_test();
bool div_test();
bool pow_test();
bool inverse_test();
/* polynomials operations */
bool poly_scale_test();
bool poly_add_test();
bool poly_mul_test();
bool poly_div_test();
bool poly_eval_test();
void run_tests();
} /* end of gf::test namespace */

} /* end of gf namespace */
#endif // GF_H

