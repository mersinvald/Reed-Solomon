# Reed-Solomon
Reed Solomon BCH encoder and decoder library

## Overview

This RS implementation was designed for embedded purposes, so all the memory allocations performed on the stack.<br>
If somebody want to reimplement memory management with heap usege, pull requests are welcome

## Build

To build the library simply run <b>make</b> in the folder with cloned repo<br>

## Usage

All the Reed-Solomon code is in folder **include**, you need to just include header files, GF ariphmetics is separated in both .hpp and .cpp, so it have to be included in your project too.
Also, **make** will build the static library with GF ariphmetics functions.

Template class ReedSolomon accepts two template arguements: message length and ecc length. <br>
Simple example: <br>
```
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
```

## Regards

Huge thanks to authors of wikiversity <br>
https://en.wikiversity.org/wiki/Reed–Solomon_codes_for_coders
