/* Author: Mike Lubinets (aka mersinvald)
 * Date: 29.12.15
 *
 * See LICENSE */

#include <iostream>
#include "gftest.hpp"
#include "rstest.hpp"

using namespace std;

int main() {
    TestSuite *tests[] {
        new GFtest,
        new RStest
    };

    for(uint i = 0 ; i < sizeof(tests) / sizeof(tests[0]); i++) {
        tests[i]->run_tests();
    }
}

