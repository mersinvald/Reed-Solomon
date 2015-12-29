/* Author: Mike Lubinets (aka mersinvald)
 * Date: 29.12.15
 *
 * See LICENSE */

#include "gftest.hpp"

bool
GFtest::run_tests() {
    bool (*tests[])(std::string&) = {
        test_add,
        test_mul,
        test_div,
        test_pow,
        test_inverse,
        test_poly_scale,
        test_poly_add,
        test_poly_mul,
        test_poly_div,
        test_poly_eval
    };

    for(uint i = 0; i < 10; i++) {
        if(!test(tests[i])) return false;
    }
    return true;
}

bool
GFtest::test_add(std::string &name) {
    name = "RS::gf::add";

    const uint testcount = 256;
    uint8* leftops  = (uint8*) RS::gf::log;
    uint8* rightops = (uint8*) RS::gf::exp;

    uint8 answers[testcount] = {
        1, 2, 5, 17, 18, 18, 90, 70, 30, 229, 71, 6, 214, 239, 212, 109, 72, 252, 205, 84, 128, 248, 5, 72, 147, 194, 111, 244, 208, 56,
        44, 177, 152, 173, 43, 179, 196, 110, 155, 20, 95, 71, 59, 173, 30, 211, 29, 102, 91, 57, 199, 119, 126, 15, 169, 25, 148, 32,
        96, 170, 244, 139, 172, 7, 89, 1, 234, 160, 255, 242, 110, 65, 135, 82, 172, 188, 14, 173, 90, 120, 203, 55, 71, 117, 228, 64,
        106, 194, 15, 51, 204, 255, 216, 142, 55, 162, 199, 237, 245, 37, 210, 106, 58, 230, 102, 32, 28, 60, 42, 56, 221, 243, 75, 65, 165,
        227, 242, 248, 190, 184, 117, 162, 9, 105, 228, 192, 193, 155, 130, 103, 238, 171, 52, 237, 185, 164, 40, 212, 255, 175, 181, 208, 212,
        76, 75, 232, 3, 94, 116, 28, 225, 214, 88, 214, 171, 171, 199, 245, 62, 93, 209, 238, 110, 56, 83, 45, 240, 179, 108, 98, 64, 1, 167,
        10, 79, 158, 17, 141, 120, 224, 130, 27, 63, 90, 17, 11, 87, 143, 226, 58, 239, 227, 157, 52, 113, 188, 127, 246, 163, 120, 216, 47,
        57, 12, 162, 171, 60, 80, 61, 3, 98, 224, 80, 111, 172, 69, 56, 251, 173, 231, 23, 137, 180, 83, 217, 125, 23, 32, 161, 211, 84, 164, 252,
        6, 237, 0, 177, 254, 39, 193, 99, 246, 101, 148, 28, 14, 98, 107, 111, 224, 152, 50, 5, 23, 214, 174
    };

    for(uint i = 0; i < testcount; i++) {
        if(!compare(RS::gf::add(leftops[i], rightops[i]), answers[i])) return false;
    }
    return true;
}

// TODO Implement other tests

bool
GFtest::test_mul(std::string &name) {
    name = "RS::gf::mul";
    return true;
}

bool
GFtest::test_div(std::string &name) {
    name = "RS::gf::div";
    return true;
}

bool
GFtest::test_pow(std::string &name) {
    name = "RS::gf::pow";
    return true;
}

bool
GFtest::test_inverse(std::string &name) {
    name = "RS::gf::inverse";
    return true;
}

bool
GFtest::test_poly_scale(std::string &name) {
    name = "RS::gf::poly_scale";
    return true;
}

bool
GFtest::test_poly_add(std::string &name) {
    name = "RS::gf::poly_add";
    return true;
}

bool
GFtest::test_poly_mul(std::string &name) {
    name = "RS::gf::poly_mul";
    return true;
}

bool
GFtest::test_poly_div(std::string &name) {
    name = "RS::gf::poly_div";
    return true;
}

bool
GFtest::test_poly_eval(std::string &name) {
    name = "RS::gf::poly_eval";
    return true;
}
