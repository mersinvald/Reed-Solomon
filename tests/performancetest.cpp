#include "performancetest.hpp"
#include <chrono>

using namespace std::chrono;

PerformanceTest::Data_t PerformanceTest::data = {};
RS::ReedSolomon<TEST_DATA_SIZE, TEST_ECC_SIZE> PerformanceTest::rs;

PReport
PerformanceTest::run_tests(uint32_t laps_cnt, uint32_t cnt_per_lap) {
    assert(laps_cnt < 64 && cnt_per_lap < 64);
    std::cout << "Starting performance tests\n";
    PReport enc_rep;
    PReport dec_rep;

    /// Init data
    for(uint32_t i = 0; i < TEST_DATA_SIZE; i++) {
        data.msg[i] = i;
        data.err_msg[i] = i;
    }

    /// Prepare corrupted message
    for(uint32_t i = 0; i < TEST_ECC_SIZE / 2; i++) {
        data.err_msg[i+1] = 0;
    }

    auto lap_time = []() -> uint32_t {
        static milliseconds last_time = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
        );

        milliseconds time = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
        );

        milliseconds diff = time - last_time;
        last_time = time;

        return diff.count();
    };

    /// Run 3 times
    for(uint32_t i = 0; i < laps_cnt; i++) {
        lap_time(); /// init static counter
        for(uint32_t i = 0; i < cnt_per_lap; i++) {
            encoder_atom();
        }
        enc_rep.laps[i] = lap_time();

        lap_time(); /// init static counter
        for(uint32_t i = 0; i < cnt_per_lap; i++) {
            decoder_atom();
        }
        dec_rep.laps[i] = lap_time();
    }

    /// Calculate average
    enc_rep.average[0] = 0;
    dec_rep.average[0] = 0;
    for(uint32_t i = 0; i < laps_cnt; i++) {
        enc_rep.average[0] += enc_rep.laps[i];
        dec_rep.average[0] += dec_rep.laps[i];
    }
    enc_rep.average[0] /= laps_cnt;
    dec_rep.average[0] /= laps_cnt;

    return PReport {
        .overall = enc_rep.average[0] + dec_rep.average[0],
        .average = {enc_rep.average[0], dec_rep.average[0]},
        .count = 2
    };
}

void
PerformanceTest::encoder_atom() {
    rs.EncodeBlock(data.msg, data.ecc);
}

void
PerformanceTest::decoder_atom() {
    rs.DecodeBlock(data.err_msg, data.ecc, data.rep_msg);
}
