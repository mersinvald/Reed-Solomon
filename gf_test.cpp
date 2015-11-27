#include "gf.hpp"
#include <iostream>
#include <memory.h>
using namespace std;

namespace gf {

namespace test {
void run_tests(){
    cout << "Testing gf::_inner::bit_len():\t\t" << ((gf::test::_inner::bit_len_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::_inner::cl_mult():\t\t" << ((gf::test::_inner::cl_mult_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::_inner::cl_div():\t\t" << ((gf::test::_inner::cl_div_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::mult_noLUT():\t\t" << ((gf::test::mult_noLUT_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::mult_noLUT_RPM():\t\t" << ((gf::test::mult_noLUT_RPM_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::init_tables():\t\t" << ((gf::test::init_tables_test()) ? "SUCCESS" : "FAILURE") << endl;

    cout << "\n";
    cout << "Testing gf::mul():\t\t\t" << ((gf::test::mul_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::div():\t\t\t" << ((gf::test::div_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::pow():\t\t\t" << ((gf::test::pow_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::inverse():\t\t\t" << ((gf::test::inverse_test()) ? "SUCCESS" : "FAILURE") << endl;

    cout << "\n";
    cout << "Testing gf::poly_scale():\t\t" << ((gf::test::poly_scale_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::poly_add():\t\t\t" << ((gf::test::poly_add_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::poly_mul():\t\t\t" << ((gf::test::poly_mul_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::poly_div():\t\t\t" << ((gf::test::poly_div_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::poly_div():\t\t\t" << ((gf::test::poly_div_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::poly_div():\t\t\t" << ((gf::test::poly_div_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::poly_div():\t\t\t" << ((gf::test::poly_div_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::poly_eval():\t\t" << ((gf::test::poly_eval_test()) ? "SUCCESS" : "FAILURE") << endl;
}

bool mult_noLUT_test(){
    bool success = true;
    int a = 0b10001001;
    int b = 0b00101010;

    int mul_only       = mult_noLUT(a, b, 0);
    int mul_mod_reduct = mult_noLUT(a, b, 0x11d);
    if(mul_only         != 0b1010001111010) success = false;
    if(mul_mod_reduct   != 0b11000011)      success = false;

    return success;
}

bool mult_noLUT_RPM_test(){
    int a = 0b10001001;
    int b = 0b00101010;

    int right_ans = 0b11000011;
    int ans = gf::mult_noLUT_RPM(a, b, 0x11d);

    if(right_ans == ans) return true;
    return false;
}

bool mul_test(){
    int a[10] = { 11, 81, 54, 239, 173, 200, 24, 255, 11, 81 };
    int b[10] = { 12, 55, 21, 44, 33, 12, 66, 99, 227, 33 };
    int r_ans[10] = {116, 33, 169, 28, 169, 9, 126, 187, 115, 163};
    int ans[10];

    for(int i = 0; i < 10; i++){
        ans[i] = mul(a[i], b[i]);
    }

    if(memcmp(r_ans, ans, 10 * sizeof(uint8)) != 0) return false;
    return true;
}

bool div_test(){
    int a = 0b10001001;
    int b = 0b00101010;

    int right_ans = 0b11011100; // 220d
    int ans = gf::div(a, b);

    if(right_ans == ans) return true;
    return false;
}

bool pow_test(){
    int x = 2;
    int16_t cases[] = {
        234, 1, 24, 65, 97, 155,
        -40, -30, -1, -140, -254
    };
    uint8 right_answers[] = {
        251, 2, 143, 190, 175, 114,
        239, 36, 142, 124, 2
    };
    uint8 answers[11];

    for(uint i = 0; i < 11; i++)
        answers[i] = pow(x, cases[i]);

    if(memcmp(right_answers, answers, sizeof(answers)) == 0) return true;

    cout << "pow(): \nexpected\t";
    for(int i = 0; i < 11; i++)
        cout << (int)right_answers[i] << " ";
    cout << "\ngot\t\t";
    for(uint i = 0; i < 11; i++)
        cout << (int)answers[i] << " ";
    cout << endl;
    return false;

    return false;
}

bool inverse_test(){
    int x = 0b10001001;

    int right_ans = 0b110001; // 49d
    int ans = gf::inverse(x);

    if(right_ans == ans) return true;
    return false;
}

bool poly_scale_test(){
    size_t as = 5;
    uint8 pa[as] = {1, 0, 7, 6, 4};
    uint8 scalar = 8;

    uint8 right_ans[5] = {8, 0, 56, 48, 32};

    size_t ans_size;
    uint8  *ans = poly_scale(pa, as, scalar, &ans_size);

    if(ans_size != 5) return false;
    if(memcmp(right_ans, ans, 5 * sizeof(uint8)) != 0) return false;
    return true;
}

bool poly_add_test(){
    size_t as = 5,
           bs = 3;
    uint8 pa[as] = {1, 0, 7, 6, 4};
    uint8 pb[bs] = {1, 6, 3};

    uint8 right_ans[5] = {1, 0, 6, 0, 7};

    size_t ans_size;
    uint8  *ans = poly_add(pa, as, pb, bs, &ans_size);

    if(ans_size != 5) return false;
    if(memcmp(right_ans, ans, 5 * sizeof(uint8)) == 0) return true;
    return false;
}

bool poly_mul_test(){
    size_t as = 5,
           bs = 3;
    uint8 pa[as] = {1, 0, 7, 6, 4};
    uint8 pb[bs] = {1, 6, 3};

    uint8 right_ans[7] = {1, 6, 4, 20, 25, 18, 12};

    size_t ans_size;
    uint8  *ans = poly_mul(pa, as, pb, bs, &ans_size);

    if(ans_size != 7) goto fail;
    if(memcmp(right_ans, ans, 7 * sizeof(uint8)) == 0) return true;

fail:
    cout << "poly_mul: \nexpected\t";
    for(uint i = 0; i < 7; i++)
        cout << (int)right_ans[i] << " ";
    cout << "\ngot\t\t";
    for(uint i = 0; i < ans_size; i++)
        cout << (int)ans[i] << " ";
    cout << endl;
    return false;
}

bool poly_div_test(){
    size_t as = 5,
           bs = 3;
    uint8 pa[as] = {1, 0, 7, 6, 4};
    uint8 pb[bs] = {1, 6, 3};

    uint8 right_ans[2] = {108, 52};

    size_t ans_size;
    uint8  *ans = poly_div(pa, as, pb, bs, &ans_size);

    if(memcmp(right_ans, ans, 2 * sizeof(uint8)) == 0) return true;

    cout << "poly_div: \nexpected\t";
    for(int i = 0; i < 2; i++)
        cout << (int)right_ans[i] << " ";
    cout << "\ngot\t\t";
    for(uint i = 0; i < ans_size; i++)
        cout << (int)ans[i] << " ";
    cout << endl;
    return false;
}

bool poly_eval_test(){
    size_t as = 5;
    uint8 pa[as] = {1, 0, 7, 6, 4};
    uint8 x = 2;

    uint8 right_ans = 4;
    uint8 ans = poly_eval(pa, as, x);
    if(ans == right_ans) return true;
    return false;
}

bool init_tables_test(){
    static u_int8_t log_right[256] = {
        0, 0, 1, 25, 2, 50, 26, 198, 3, 223, 51, 238, 27, 104, 199, 75,
        4, 100, 224, 14, 52, 141, 239, 129, 28, 193, 105, 248, 200, 8, 76, 113,
        5, 138, 101, 47, 225, 36, 15, 33, 53, 147, 142, 218, 240, 18, 130, 69,
        29, 181, 194, 125, 106, 39, 249, 185, 201, 154, 9, 120, 77, 228, 114, 166,
        6, 191, 139, 98, 102, 221, 48, 253, 226, 152, 37, 179, 16, 145, 34, 136,
        54, 208, 148, 206, 143, 150, 219, 189, 241, 210, 19, 92, 131, 56, 70, 64,
        30, 66, 182, 163, 195, 72, 126, 110, 107, 58, 40, 84, 250, 133, 186, 61,
        202, 94, 155, 159, 10, 21, 121, 43, 78, 212, 229, 172, 115, 243, 167, 87,
        7, 112, 192, 247, 140, 128, 99, 13, 103, 74, 222, 237, 49, 197, 254, 24,
        227, 165, 153, 119, 38, 184, 180, 124, 17, 68, 146, 217, 35, 32, 137, 46,
        55, 63, 209, 91, 149, 188, 207, 205, 144, 135, 151, 178, 220, 252, 190, 97,
        242, 86, 211, 171, 20, 42, 93, 158, 132, 60, 57, 83, 71, 109, 65, 162,
        31, 45, 67, 216, 183, 123, 164, 118, 196, 23, 73, 236, 127, 12, 111, 246,
        108, 161, 59, 82, 41, 157, 85, 170, 251, 96, 134, 177, 187, 204, 62, 90,
        203, 89, 95, 176, 156, 169, 160, 81, 11, 245, 22, 235, 122, 117, 44, 215,
        79, 174, 213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168, 80, 88, 175,
    };

    static u_int8_t exp_right[512] = {
        1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192,
        157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161,
        95, 190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163, 91, 182, 113, 226,
        217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102, 204,
        133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115,
        230, 209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65,
        130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9,
        18, 36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142, 1,
        2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192, 157,
        39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95,
        190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217,
        175, 67, 134, 17, 34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102, 204, 133,
        23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230,
        209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65, 130,
        25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18,
        36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142, 1, 2,
    };

    init_tables();

    bool fail_log = false,
         fail_exp = false;

    if(memcmp(log, log_right, 256 * sizeof(u_int8_t)) != 0) fail_log = true;
    if(memcmp(exp, exp_right, 512 * sizeof(u_int8_t)) != 0) fail_exp = true;

    if(!fail_log && !fail_exp) return true;

    if(fail_exp) {
        std::cout << "init_tables(): Wrong exp table generated\n";
        for(int i = 0; i < 512; i++){
            if(exp_right[i] != exp[i]){
                std::printf("exp[%i]: expected %i, got %i\n", i, exp_right[i], exp[i]);
            }
        }
    }
    if(fail_log){
        std::cout << "init_tables(): Wrong log table generated\n";
        for(int i = 0; i < 256; i++){
            if(log_right[i] != log[i]){
                std::printf("log[%i]: expected %i, got %i\n", i, log_right[i], log[i]);
            }
        }
    }
    return false;
}

} /* end of test namespace */

namespace test {
namespace _inner {
bool bit_len_test(){
    int a = 0b11001010110;
    /*        ^
     *       109876543210 */
    int right_ans = 11;
    int ans = gf::_inner::bit_len(a);
    if(right_ans == ans) return true;
    return false;
}

bool cl_mult_test(){
    int a = 0b10001001;
    int b = 0b00101010;
    int right_ans = 0b1010001111010;
    int ans = gf::_inner::cl_mult(a, b);

    if(right_ans == ans) return true;
    return false;
}

bool cl_div_test(){
    int dividend = 0b1010001111010;
    int divisor  = 0b100011101;
    int right_ans = 0b11000011;
    int ans = gf::_inner::cl_div(dividend, divisor);

    if(right_ans == ans) return true;
    return false;
}
} /* end of test::_inner namespace */
}
} /* end of gf namespace */
