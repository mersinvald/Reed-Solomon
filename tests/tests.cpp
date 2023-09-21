/* Author: Mike Lubinets (aka mersinvald)
 * Date: 29.12.15
 *
 * See LICENSE */

#include <iostream>
#include "gftest.hpp"
#include "rstest.hpp"
#include "performancetest.hpp"

using namespace std;

#define COUNT_PER_PTEST_LAP 63
#define COUNT_OF_LAPS       22

int main() {
    GFtest::run_tests();
    RStest::run_tests();

    PReport performance = PerformanceTest::run_tests(COUNT_OF_LAPS, COUNT_PER_PTEST_LAP);
    std::cout << "Average of " << COUNT_OF_LAPS << "x" << COUNT_PER_PTEST_LAP << " for encoder: " << performance.average[0] << "ms\n";
    std::cout << "Average of " << COUNT_OF_LAPS << "x" << COUNT_PER_PTEST_LAP << " for decoder: " << performance.average[1] << "ms\n";
}

