#ifndef RSTEST_H
#define RSTEST_H
#include "testsuite.hpp"
#include <rs.hpp>

class RStest : public TestSuite
{
public:
    RStest() {}

    bool run_tests();

    static bool test_encode(std::string& name);
    static bool test_decode(std::string& name);
    static bool test_stress(std::string& name);
};

#endif // RSTEST_H
