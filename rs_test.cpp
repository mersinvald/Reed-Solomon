#include <rs.hpp>
#include <memory.h>
#include <iostream>

namespace RS {
namespace test {

template<typename T>
void print_array(T *array, int count){
    for(int i = 0; i < count; i++){
        std::cout << (int)array[i] << " ";
    }
    std::cout << std::endl;
}

void run_tests(){
    std::cout << "Testing RS::generator_poly(): \t\t" << ((RS::test::generator_poly_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing RS::encode_msg(): \t\t" << ((RS::test::encode_msg_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "\n";

    std::cout << "Testing RS::calc_syndromes(): \t\t" << ((RS::test::calc_syndromes_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing RS::check(): \t\t\t" << ((RS::test::check_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing RS::find_errata_locator(): \t" << ((RS::test::find_errata_locator_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing RS::find_error_evaluator(): \t" << ((RS::test::find_error_evaluator_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing RS::correct_errata(): \t\t" << ((RS::test::correct_errata_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing RS::find_error_locator(): \t\t" << ((RS::test::find_error_locator_test()) ? "SUCCESS" : "FAILURE") << std::endl;


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

bool find_errata_locator_test() {
    // uint8 err_msg[24] = {0, 210, 117, 71, 0, 23, 0, 6, 39, 38, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    // uint8 err_pos[3] = {0, 4, 6};
    uint8 coef_pos[3] = {23, 19, 17};
    uint8 right_eloc[4] = {210, 250, 11, 1};

    size_t eloc_size;
    uint8 *err_loc = find_errata_locator(coef_pos, 3, &eloc_size);

    if(eloc_size != 4) return false;
    if(memcmp(err_loc, right_eloc, eloc_size * sizeof(uint8)) == 0) return true;

    std::cout << "find_errata_locator():\n"
                 "expected:\t";
    for(uint i = 0; i < 4; i++) {
        std::cout << (int)right_eloc[i] << " ";
    }
    std::cout << "\ngot:\t\t";
    for(uint i = 0; i < eloc_size; i++) {
        std::cout << (int)err_loc[i] << " ";
    }
    return false;
}

bool find_error_evaluator_test() {
    uint8 err_msg[24] = {0, 210, 117, 71, 0, 23, 0, 6, 39, 38, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    uint8 right_reeval[4] = {0, 175, 42, 4};
    uint8 coef_pos[3] = {23, 19, 17};

    size_t synd_size;
    uint8* synd = calc_syndromes(err_msg, 24, 8, &synd_size);

    size_t eloc_size;
    uint8* eloc = find_errata_locator(coef_pos, 3, &eloc_size);

    uint8 rsynd[synd_size];
    for(int i = synd_size, j = 0; i >= 0; --i, j++)
        rsynd[j] = synd[i];

    size_t eeval_size;
    uint8* reeval = find_error_evaluator(rsynd, synd_size, eloc, eloc_size, eloc_size-1, &eeval_size);

    if(eeval_size != 4) return false;
    if(memcmp(reeval, right_reeval, eeval_size * sizeof(uint8)) == 0) return true;

    std::cout << "find_error_evaluator():\n"
                 "expected:\t";
    for(uint i = 0; i < 4; i++) {
        std::cout << (int)right_reeval[i] << " ";
    }
    std::cout << "\ngot:\t\t";
    for(uint i = 0; i < eeval_size; i++) {
        std::cout << (int)reeval[i] << " ";
    }
    std::cout << "\n";

    return false;
}

bool correct_errata_test() {
    uint8 right_msg[24] = {64, 210, 117, 71, 118, 23, 50, 6, 39, 38, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    /* adding the erasure to test message */
    uint8 err_msg[24] = {0, 210, 117, 71, 0, 23, 0, 6, 39, 38, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    uint8 err_pos[3] = {0, 4, 6};

    size_t synd_size;
    uint8 *synd = calc_syndromes(err_msg, 24, 8, &synd_size);


    size_t corrected_size;
    uint8 *corrected = correct_errata(err_msg, 24, synd, synd_size, err_pos, 3, &corrected_size);

    if(corrected_size != 24) return false;
    if(memcmp(corrected, right_msg, corrected_size * sizeof(uint8)) == 0) return true;

    std::cout << "correct_errata():\n"
                 "expected:\t";
    for(uint i = 0; i < 24; i++) {
        std::cout << (int)right_msg[i] << " ";
    }
    std::cout << "\ngot:\t\t";
    for(uint i = 0; i < corrected_size; i++) {
        std::cout << (int)corrected[i] << " ";
    }
    std::cout << "\n";

    return false;
}

bool find_error_locator_test() {
    uint8 case1[] = {0, 194, 192, 156, 139, 136, 252, 81, 95};
    uint8 case2[] = {0, 4, 6, 90, 77, 78, 58, 151, 153};
    uint8 case2_eloc[] = {0, 4};
    int   nsym = 8;

    uint8 ans1[] = {210, 40, 241, 10, 1};
    uint8 ans2[] = {236, 176, 93, 4};

    size_t ta1_size;
    uint8* testans1 = find_error_locator(case1, sizeof(case1) / sizeof(uint8), 8, &ta1_size);

    size_t ta2_size;
    uint8* testans2 = find_error_locator(case2,  sizeof(case2) / sizeof(uint8), 8, &ta2_size, case2_eloc, 2);

    if(memcmp(testans1, ans1, sizeof(ans1)) != 0) {
        std::cout << "find_error_locator():\nexpected: ";
        print_array(ans1, 5);
        std::cout << "\n got: ";
        print_array(testans1, ta1_size);
        return false;
    }

    if(memcmp(testans2, ans2, sizeof(ans2)) != 0) {
        std::cout << "find_error_locator():\nexpected: ";
        print_array(ans2, 4);
        std::cout << "\n got: ";
        print_array(testans2, ta1_size);
        return false;
    }
    return true;
}

}
}
