#ifndef GF_H
#define GF_H
#include <stdlib.h>
#include "polyhandler.h"

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
 * @return x + y */
uint8 add(uint8 x, uint8 y);

/* ##### GF substraction ###### */
/* @brief Substraction in Galua Fields
 * @param x - left operand
 * @param y - right operand
 * @return x - y */
uint8 sub(uint8 x, uint8 y);

/* DEPRECATED AND SLOW
 * @brief Multiplication in Galua Fields without lookup table (slow)
 * @param x    - left operand
 * @param y    - right operand
 * @param prim - primitive polinominal of GF
 * @return x * y */
int mult_noLUT(uint8 x, uint8 y, int prim = 0);

/* @brief Multiplication in Galua Fields w/o lookup table, Russian Peasant Multiplication algorithm
 * @param x                 - left operand
 * @param y                 - right operand
 * @param prim              - primitive polinominal of GF
 * @param field_charac_full - some mathematical stuff I don't understand
 * @param carryless         - do carryless multiplication?
 * @return x * y */
uint8 mult_noLUT_RPM(int x, int y, int prim = 0, int field_charac_full = 256, bool carryless = true);

/* @brief Multiplication in Galua Fields w/ lookup table (needs to be initialized)
 * @param x - left operand
 * @param y - right operand
 * @return x * y */
uint8 mul(ushort x, ushort y);

/* @brief Division in Galua Fields w/ lookup table (needs to be initialized)
 * @param x - dividend
 * @param y - divisor
 * @return x / y */
uint8 div(uint8 x, uint8 y);

/* @brief X in power Y w/ lookup table (needs to be initialized)
 * @param x     - operand
 * @param power - power
 * @return x^power */
uint8 pow(uint8 x, int power);

/* @brief Inversion in Galua Fields w/ lookup table (needs to be initialized)
 * @param x - number
 * @return inversion of x */
uint8 inverse(uint8 x);

/* ##########################
 * # POLYNOMIALS OPERATIONS #
 * ########################## */

/* @brief Multiplication polynomial by scalar
 * @param &p    - source polynomial
 * @param &newp - destination polynomial
 * @param x     - scalar */
void poly_scale(const Poly &p, Poly &newp, int x);

/* @brief Addition of two polynomials
 * @param &p    - right operand polynomial
 * @param &q    - left operand polynomial
 * @param &newp - destination polynomial */
void poly_add(const Poly &p, const Poly &q, Poly &newp);

/* @brief Multiplication of two polynomials
 * @param &p    - right operand polynomial
 * @param &q    - left operand polynomial
 * @param &newp - destination polynomial */
void poly_mul(const Poly &p, const Poly &q, Poly &newp);

/* @brief Division of two polynomials
 * @param &p    - right operand polynomial
 * @param &q    - left operand polynomial
 * @param &newp - destination polynomial */
void poly_div(const Poly &p, const Poly &q, Poly &newp);

/* @brief Evaluation of polynomial in x
 * @param &p - polynomial to evaluate
 * @param x  - evaluation point */
int poly_eval(const Poly &p, int x);

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

