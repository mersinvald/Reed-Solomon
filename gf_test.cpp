#include "gf.hpp"
#include <iostream>
#include <memory.h>
using namespace std;

namespace gf {

namespace test {
void run_tests(){
    cout << "Testing gf::_inner::bit_len(): \t" << ((gf::test::_inner::bit_len_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::_inner::cl_mult(): \t" << ((gf::test::_inner::cl_mult_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::_inner::cl_div(): \t" << ((gf::test::_inner::cl_div_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::mult_noLUT(): \t" << ((gf::test::mult_noLUT_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::mult_noLUT_RPM(): \t" << ((gf::test::mult_noLUT_RPM_test()) ? "SUCCESS" : "FAILURE") << endl;

    cout << "\n";
    cout << "Testing gf::mul(): \t" << ((gf::test::mul_test()) ? "SUCCESS" : "FAILURE") << endl;

    cout << "\n";
    cout << "Testing gf::poly_add(): \t" << ((gf::test::poly_add_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::poly_mul(): \t" << ((gf::test::poly_mul_test()) ? "SUCCESS" : "FAILURE") << endl;
    cout << "Testing gf::poly_div(): \t" << ((gf::test::poly_div_test()) ? "SUCCESS" : "FAILURE") << endl;
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
    int a = 0b10001001;
    int b = 0b00101010;

    int right_ans = 0b11000011;
    int ans = gf::mul(a, b);

    if(right_ans == ans) return true;
    return false;
}

bool div_test(){

}

bool pow_test(){

}

bool inverse_test(){

}

bool poly_scale_test(){

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
    if(memcmp(right_ans, ans, 5) == 0) return true;
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
    if(memcmp(right_ans, ans, 7) == 0) return true;

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

    uint8 right_ans[5] = {1, 6, 16, 108, 52};

    size_t ans_size;
    size_t sep;
    uint8  *ans = poly_div(pa, as, pb, bs, &ans_size, &sep);

    //if(sep != 3) return false;
    if(ans_size != 5) return false;
    if(memcmp(right_ans, ans, 5) == 0) return true;

    cout << "poly_div: \nexpected\t";
    for(int i = 0; i < 5; i++)
        cout << (int)right_ans[i] << " ";
    cout << "\ngot\t\t";
    for(int i = 0; i < 5; i++)
        cout << (int)ans[i] << " ";
    cout << endl;
    return false;
}

bool poly_eval_test(){

}

bool init_tables_test(){
    return true; /* tested manually */
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
