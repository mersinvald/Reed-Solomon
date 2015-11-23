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



}

#endif // RS_HPP

