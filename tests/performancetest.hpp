#ifndef PERFORMANCETEST_HPP
#define PERFORMANCETEST_HPP
#include "testsuite.hpp"
#include "string.h"
#include "rs.hpp"

#define TEST_DATA_SIZE 200
#define TEST_ECC_SIZE  40

class PerformanceTest : public TestSuite
{
public:
    static PReport run_tests(uint32_t laps_cnt, uint32_t cnt_per_lap);

    static void encoder_atom();
    static void decoder_atom();

private:
    static struct Data_t {
        uint8_t msg    [TEST_DATA_SIZE];
        uint8_t err_msg[TEST_DATA_SIZE];
        uint8_t rep_msg[TEST_DATA_SIZE];
        uint8_t ecc[TEST_ECC_SIZE];
    } data;

    static RS::ReedSolomon<TEST_DATA_SIZE, TEST_ECC_SIZE> rs;
};

#endif // PERFORMANCETEST_H
