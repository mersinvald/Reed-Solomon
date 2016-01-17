/* Author: Mike Lubinets (aka mersinvald)
 * Date: 29.12.15
 *
 * See LICENSE */

#ifndef RSTEST_H
#define RSTEST_H
#include "testsuite.hpp"
#include <rs.hpp>

class RStest : public TestSuite
{
public:
    static Report run_tests();

    static bool test_encode(std::string& name);
    static bool test_decode(std::string& name);
    static bool test_stress(std::string& name);
};

#endif // RSTEST_H
