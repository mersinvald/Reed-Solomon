/* Author: Mike Lubinets (aka mersinvald)
 * Date: 29.12.15
 *
 * See LICENSE */

#include <iostream>
#include "gftest.hpp"
#include "rstest.hpp"

using namespace std;

int main() {
    GFtest::run_tests();
    RStest::run_tests();
}

