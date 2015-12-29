# Reed-Solomon
Reed Solomon BCH encoder and decoder

## Overview

This RS implementation was designed for embedded purposes, so all the memory allocations performed on the stack.<br>
If somebody want to reimplement memory management with heap usege, pull requests are welcome

## Build

To build the library simply run <b>mkdir -p build/obj && make</b> in the folder with cloned repo<br>
Also qtcreator .pro files are present.

## Usage

Template class ReedSolomon has two template arguements: message length and ecc length. <br>
Simple example: <br>
<code>
    char message[] = "Some very important message ought to be delivered";
    const int msglen = sizeof(message);
    const int ecclen = 8;
    
    char repaired[msglen];
    char encoded[msglen + ecclen];


    RS::ReedSolomon<msglen, ecclen> rs;

    rs.Encode(message, encoded);

    // Corrupting first 8 bytes of message (any 4 bytes can be repaired, no more)
    for(uint i = 0; i < ecclen / 2; i++) {
        encoded[i] = 'E';
    }

    rs.Decode(encoded, repaired);

    std::cout << "Original:  " << message  << std::endl;
    std::cout << "Corrupted: " << encoded  << std::endl;
    std::cout << "Repaired:  " << repaired << std::endl;

    std::cout << ((memcmp(message, repaired, msglen) == 0) ? "SUCCESS" : "FAILURE") << std::endl;
</code>
