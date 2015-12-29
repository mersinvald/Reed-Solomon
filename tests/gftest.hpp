/* Author: Mike Lubinets (aka mersinvald)
 * Date: 29.12.15
 *
 * See LICENSE */

#ifndef GFTEST_H
#define GFTEST_H
#include "testsuite.hpp"
#include <gf.hpp>

class GFtest : public TestSuite {
public:
    GFtest() {}

    bool run_tests();

    static bool test_add(std::string& name);
    static bool test_sub(std::string& name);
    static bool test_mul(std::string& name);
    static bool test_div(std::string& name);
    static bool test_pow(std::string& name);
    static bool test_inverse(std::string& name);

    static bool test_poly_scale(std::string& name);
    static bool test_poly_add  (std::string& name);
    static bool test_poly_mul  (std::string& name);
    static bool test_poly_div  (std::string& name);
    static bool test_poly_eval (std::string& name);
};

#endif // GFTEST_H
