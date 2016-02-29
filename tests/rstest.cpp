/* Author: Mike Lubinets (aka mersinvald)
 * Date: 29.12.15
 *
 * See LICENSE */

#include "rstest.hpp"
#include <rs.hpp>

Report
RStest::run_tests() {
    bool (*tests[])(std::string&) = {
        test_encode,
        test_decode,
        test_stress
    };

    Report rep = {3, 0};

    for(uint i = 0; i < rep.overall; i++) {
        if(test(tests[i])) rep.passed++;
    }

    std::cout << "RSTest: " << rep.passed << "/" << rep.overall << " tests passed.\n\n";

    return rep;
}

bool
RStest::test_encode(std::string &name) {
    INIT_TESTCASE;
    RS::ReedSolomon<30, 8> rs;

    char message[30];
    char encoded[38];

    uint8_t right[38] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        99, 26, 219, 193, 9, 94, 186, 143
    };

    memcpy(message, right, 30);

    rs.Encode(message, encoded);

    SUBTEST(compare((uint8_t*)encoded, (uint8_t*)right, 38));
    RETURN;
}

bool
RStest::test_decode(std::string &name) {
    INIT_TESTCASE;
    RS::ReedSolomon<30, 8> rs;

    char message[30];
    char right[30] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29
    };

    // Test clean message
    uint8_t clean[38] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        99, 26, 219, 193, 9, 94, 186, 143
    };
    SUBTEST(rs.Decode(clean, message) == 0);
    SUBTEST(compare(message, right, 30));

    // Test corrupted message
    uint8_t corrupted[38] = {
        0, 1, 2, 3, 4, 5, 6, 7, 0, 9, 10, 11, 12, 13, 14, 0, 16, 17, 18, 19, 0, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        99, 26, 219, 193, 9, 94, 0, 143
    };

    SUBTEST(rs.Decode(corrupted, message) == 0);
    SUBTEST(compare(message, right, 30));

    RETURN
}

// TODO make complex encoder-decder test

bool
RStest::test_stress(std::string &name) {
    INIT_TESTCASE;

    SUBTEST(true);

    RETURN;
}

