#include <rs.hpp>
#include <memory.h>
#include <iostream>

namespace RS {
namespace test {

void run_tests(){
    std::cout << "Testing RS::generator_poly(): \t" << ((RS::test::generator_poly_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing RS::encode_msg(): \t" << ((RS::test::encode_msg_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "\n";

    std::cout << "Testing RS::calc_syndromes(): \t" << ((RS::test::calc_syndromes_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing RS::check(): \t\t" << ((RS::test::check_test()) ? "SUCCESS" : "FAILURE") << std::endl;

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
    for(uint i = 0; i < omsgsize; i++) std::cout << (int)right_out_msg[i] << " ";
    std::cout << "\ngot:\t\t";
    for(uint i = 0; i < omsgsize; i++) std::cout << (int)out_msg[i] << " ";
    std::cout << std::endl;
    return false;
}

bool calc_syndromes_test(){
    uint8 enc_msg[24] = {64, 210, 117, 71, 118, 23, 50, 6, 39, 38, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    uint8 e_r_synd[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    uint8 cor_msg[24] = {64, 15, 22, 71, 118, 23, 50, 13, 39, 88, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    uint8 c_r_synd[9] = {0, 203, 98, 254, 12, 227, 127, 129, 190};

    size_t e_ss;
    size_t c_ss;
    uint8* e_synd = calc_syndromes(enc_msg, 24, 8, &e_ss);
    uint8* c_synd = calc_syndromes(cor_msg, 24, 8, &c_ss);

    bool clear_fail = false,
         corrupt_fail = false;

    if(e_ss != 9) clear_fail   = true;
    if(c_ss != 9) corrupt_fail = true;

    if(memcmp(e_r_synd, e_synd, e_ss * sizeof(uint8)) != 0) clear_fail   = true;
    if(memcmp(c_r_synd, c_synd, c_ss * sizeof(uint8)) != 0) corrupt_fail = true;

    if(clear_fail) {
        std::cout << "calc_syndromes(): clear msg syndrome calculation failed\nexpected:\t";
        for(uint i = 0; i < 9; i++) std::cout << (int)e_r_synd[i] << " ";
        std::cout << "\ngot:\t\t";
        for(uint i = 0; i < e_ss; i++) std::cout << (int)e_synd[i] << " ";
        std::cout << std::endl;
    }
    if(corrupt_fail) {
        std::cout << "calc_syndromes(): corrupted msg syndrome calculation failed\nexpected:\t";
        for(uint i = 0; i < 9; i++) std::cout << (int)c_r_synd[i] << " ";
        std::cout << "\ngot:\t\t";
        for(uint i = 0; i < c_ss; i++) std::cout << (int)c_synd[i] << " ";
        std::cout << std::endl;
    }

    if(corrupt_fail || clear_fail) return false;
    return true;
}

bool check_test() {
    uint8 enc_msg[24] = {64, 210, 117, 71, 118, 23, 50, 6, 39, 38, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    uint8 cor_msg[24] = {64, 15, 22, 71, 118, 23, 50, 13, 39, 88, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};

    bool e_r_ans = true;
    bool c_r_ans = false;

    bool e_ans = check(enc_msg, 24, 8);
    bool c_ans = check(cor_msg, 24, 8);

    bool e_fail = false;
    bool c_fail = false;

    if(e_r_ans != e_ans) e_fail = true;
    if(c_r_ans != c_ans) c_fail = true;

    if(e_fail || c_fail) return false;
    return true;
}

}
}
