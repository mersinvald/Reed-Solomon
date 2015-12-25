#include "rstest.hpp"
#include <rs.hpp>

bool
RStest::run_tests() {
    bool (*tests[])(std::string&) = {
        test_encode,
        test_decode,
        test_stress
    };

    for(uint i = 0; i < 3; i++) {
        if(!test(tests[i])) return false;
    }
    return true;
}

bool
RStest::test_encode(std::string &name) {
    name = "RS::Encode";
    RS::ReedSolomon<30, 8> rs;

    char message[30];
    char encoded[38];

    uint8 right[38] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        99, 26, 219, 193, 9, 94, 186, 143
    };

    for(uint i = 0; i < 30; i++){
        message[i] = i;
    }

    rs.Encode(message, encoded);

    return compare((uint8*)encoded, (uint8*)right, 38, 38);
}

bool
RStest::test_decode(std::string &name) {
    name = "RS::Decode";
    RS::ReedSolomon<30, 8> rs;
    uint8 encoded[38] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        99, 26, 219, 193, 9, 94, 186, 143
    };

    char right[30] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29
    };

    char message[30];

    rs.Decode(encoded, message);

    return compare(message, right, 30, 30);
}

// TODO make complex encoder-decder test

bool
RStest::test_stress(std::string &name) {
    name = "Complex";
    return true;
}

