#include <iostream>
#include "rs.hpp"
using namespace std;

#define ECC_LENGTH 8

int main() {

    char message[] = "Some very important message ought to be delivered";
    const int msglen = sizeof(message);

    char repaired[msglen];
    char encoded[msglen + ECC_LENGTH];


    RS::ReedSolomon<msglen, ECC_LENGTH> rs;

    rs.Encode(message, encoded);

    // Corrupting first 8 bytes of message (any 8 bytes can be repaired)
    for(uint i = 0; i < ECC_LENGTH / 2; i++) {
        encoded[i] = 'E';
    }

    rs.Decode(encoded, repaired);

    std::cout << "Original:  " << message  << std::endl;
    std::cout << "Corrupted: " << encoded  << std::endl;
    std::cout << "Repaired:  " << repaired << std::endl;

    std::cout << ((memcmp(message, repaired, msglen) == 0) ? "SUCCESS" : "FAILURE") << std::endl;
    return 0;
}

