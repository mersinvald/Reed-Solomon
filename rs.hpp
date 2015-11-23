#ifndef RS_HPP
#define RS_HPP
#include <stdlib.h>
typedef u_int8_t uint8;

#define MAX_MSG_SIZE(nsym) 255-nsym

namespace RS {

/* @brief Generate an irreducible generator polinomial (necessary to encode a message into Reed-Solomon)
 * @param nsym - desirable lenght of ECC
 * @return generate polynomial
 */
uint8* generator_poly(int nsym, size_t* size);

/* @brief Encode message
 * @param *msg_in - message to encode
 * @param msg_in_size - size of message
 * @param nsym - desirable lenght of ECC
 * @param *msg_out - ptr to encoded message
 * @param *msg_out_size - ptr no encoded message size
 * @return encoded message
 */
uint8* encode_msg(uint8* msg_in, size_t msg_in_size, int nsym, size_t* msg_out_size);

/* @brief Calculate endoded message syndromes
 * @param *msg - encoded, possibly corrupted message
 * @param msg_size - size of message
 * @param nsym - lenght of ECC
 * @param *synd_size - size of syndromes polynomial
 * @return syndromes polinomial
 */
uint8* calc_syndromes(uint8* msg, size_t msg_size, uint nsym, size_t* synd_size);


/* @TODO: doxygen headers */

bool check(uint8* msg, size_t msg_size, int nsym);

uint8* find_errata_locator(uint8* e_pos, size_t e_pos_size, size_t* errloc_size);

uint8* find_error_evaluator(uint8* synd, size_t synd_size, uint8* err_loc, size_t err_loc_size, int nsym, size_t* err_eval_size);

uint8* correct_errata(uint8* msg_in, size_t msg_in_size, uint8* synd, size_t synd_size, uint8* err_pos, size_t epos_size, size_t* newsize);

uint8* find_error_locator(uint8* synd, size_t synd_size, uint8* erase_loc = NULL, size_t erase_loc_size = 0, size_t erace_count = 0);

uint8* find_errors(uint8* err_loc, size_t eloc_size, size_t msg_in_size);

uint8* forney_syndromes(uint8* synd, size_t synd_size, size_t msg_in_size);

uint8* correct_msg(uint8* msg_in, size_t msg_in_size, int nsym, uint8* erase_pos = NULL, size_t epos_size = 0);




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

