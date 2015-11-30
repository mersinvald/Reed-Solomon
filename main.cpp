#include <iostream>
#include <cstring>
#include "gf.hpp"
#include "rs.hpp"
#include "polyhandler.h"
using namespace std;

template<typename T>
void print_array(T *arr, uint count){
    for(uint i = 0; i < count; i++){
        std::cout << (int)arr[i] << " ";
    }
    std::cout << std::endl;
}

int main()
{
    srand(clock());

    /* running tests */
    gf::test::run_tests();
    RS::test::run_tests();


    /* GF primitive polynomial */
    int prim = 0x11d;

    /* count of ECC bytes */
    int nsym = 8;

    /* initialize Lookup Tables */
    gf::init_tables(prim);

    char msg[60];
    for(uint i = 0; i < 60; i++) {
        msg[i] = rand() % 128;
    }

    uint msg_size = sizeof(msg);
    uint msg_len  = sizeof(msg) / sizeof(char);

    uint8 encoded_msg[msg_len + nsym];
    uint encoded_msg_size = sizeof(encoded_msg);
    uint encoded_msg_len = encoded_msg_size / sizeof(uint8);

    RS::encode_msg(msg, msg_size, encoded_msg, encoded_msg_size, nsym);

    std::cout << "Original(hex): ";
    std::cout << std::hex;
    print_array(msg, msg_len);
    std::cout << "Encoded:       ";
    print_array(encoded_msg, encoded_msg_len);


    /* corrupting 4 bytes of message
     * nsym/2 cause algorithm can repair nsym/2 unknown errors and nsym erasures (errors with known location)*/
    for(int i = 0; i < nsym/2; i++) {
        encoded_msg[i] = 0;
    }

    std::cout << "Corrupted:     ";
    print_array(encoded_msg, encoded_msg_len);

    /* repairing message */
    char repaired_msg[msg_len];
    bool ok = RS::correct_msg(encoded_msg, encoded_msg_size, repaired_msg, msg_size, nsym, NULL, 0);

    if(ok){
        std::cout << "Repaired:      ";
        print_array(repaired_msg, msg_len);
    }
    else
        std::cout << "Too many errors\n";
    std::cout << ((strcmp(msg, repaired_msg) == 0) ? "SUCCESS\n" : "FAIL\n");
}

