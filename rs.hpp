#ifndef RS_HPP
#define RS_HPP
#include <stdlib.h>
#include "polyhandler.h"
typedef u_int8_t uint8;

#define MAX_MSG_SIZE(nsym) 255-nsym

namespace RS {


/* @brief Generates an irreducible generator polynomial (necessary to encode a message into Reed-Solomon)
 * @param nsym - desirable lenght of ECC
 * @return generate polynomial */
void generator_poly(int nsym, Poly &generator, PolyHandler &PH);

/* @brief Encode message
 * @param *msg_in      - message to encode
 * @param msg_in_size  - size of message
 * @param nsym         - desirable lenght of ECC (must be < 255 - length of msg_in)
 * @param *msg_out     - ptr where to write encoded message
 * @param msg_out_size - encoded message size (must be at least msg_in_size + nsym*sizeof(uint8)) */
void encode_msg(void* msg_in, size_t msg_in_size, void* msg_out, size_t msg_out_size, int nsym);

/* @brief Correct message if needed
 * @param *msg_in      - message to correct
 * @param msg_in_size  - size of message
 * @param *msg_out     - ptr where to write decoded message
 * @param msg_out_size - encoded message size (must be at least msg_in_size - nsym*sizeof(uint8))
 * @param nsym         - count of ECC bytes
 * @param *erase_pos   - positions of erasures (if they were detected by some external algorithm)
 * @param erase_count  - count of erasures
 * @return true if decoding was successfull */
bool correct_msg(void* msg_in, size_t msg_in_size, void* msg_out, size_t msg_out_size, int nsym, uint8* erase_pos, size_t erase_count);

/* a lot of math I really don't understand, but function prototypes are pretty understandable */

void calc_syndromes(const Poly &msg, uint nsym, Poly &synd);

void find_errata_locator(const Poly &epos, Poly &errata_loc, PolyHandler &PH);

void find_error_evaluator(const Poly &synd, const Poly &errata_loc, int nsym, Poly &err_eval, PolyHandler &PH);

void correct_errata(const Poly &synd, const Poly &err_pos, const Poly &msg_in, Poly &corrected, PolyHandler &PH);

bool find_error_locator(const Poly &synd, int nsym, Poly &error_loc, PolyHandler &PH, Poly *erase_loc = NULL, size_t erase_count = 0);

bool find_errors(const Poly& error_loc, Poly& errors, size_t msg_in_size);

void forney_syndromes(const Poly &synd, const Poly &erasures_pos, Poly &forney_synd, size_t msg_in_size, PolyHandler &PH);


namespace test {

void run_tests();
bool generator_poly_test();
bool encode_msg_test();

bool calc_syndromes_test();
bool check_test();
bool find_errata_locator_test();
bool find_error_evaluator_test();
bool correct_errata_test();
bool find_error_locator_test();
bool find_errors_test();
bool forney_syndromes_test();
bool correct_msg_test();

}

}

#endif // RS_HPP

