#include <rs.hpp>
#include <memory.h>
#include <iostream>

namespace RS {
namespace test {

void run_tests(){
    std::cout << "Testing RS::generator_poly(): \t" << ((RS::test::generator_poly_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing RS::encode_msg(): \t" << ((RS::test::encode_msg_test()) ? "SUCCESS" : "FAILURE") << std::endl;

}

bool generator_poly_test(){
    int nsym = 8;

    uint8 right_gp[9] = {1, 255, 11, 81, 54, 239, 173, 200, 24};

    size_t gsize;
    uint8* gp = generator_poly(nsym, &gsize);

    if(gsize != 9) return false;
    if(memcmp(right_gp, gp, gsize * sizeof(uint8)) == 0) return true;

    std::cout << "generator_poly():\nexpected:\t";
    for(uint i = 0; i < gsize; i++) std::cout << right_gp[i] << " ";
    std::cout << "\ngot:\t\t";
    for(uint i = 0; i < gsize; i++) std::cout << gp[i] << " ";
    std::cout << std::endl;
    return false;
}

bool encode_msg_test(){
    uint8 in_msg[16] = {0x40, 0xd2, 0x75, 0x47, 0x76, 0x17, 0x32, 0x06, 0x27, 0x26, 0x96, 0xc6, 0xc6, 0x96, 0x70, 0xec};
    uint8 right_out_msg[24] = {64, 210, 117, 71, 118, 23, 50, 6, 39, 38, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};

    size_t omsgsize;
    uint8* out_msg = encode_msg(in_msg, 16, 8, &omsgsize);

    if(omsgsize != 24) return false;
    if(memcmp(right_out_msg, out_msg, omsgsize * sizeof(uint8)) == 0) return true;

    std::cout << "encode_msg():\nexpected:\t";
    for(uint i = 0; i < omsgsize; i++) std::cout << right_out_msg[i] << " ";
    std::cout << "\ngot:\t\t";
    for(uint i = 0; i < omsgsize; i++) std::cout << out_msg[i] << " ";
    std::cout << std::endl;
    return false;
}

}
}
