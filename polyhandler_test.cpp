#include "polyhandler.h"
#include <memory.h>
#include <iostream>
#include <random>
#include <time.h>

/* @TODO Rewrite these tests cause they are not valid for current version of PolyHandler */
/*
namespace test {

uint8 t1[] = {104, 101, 108, 110, 109, 3};
uint8 t2[] = {17, 9, 5, 143, 3};
uint8 t3[] = {9, 5, 3, 70, 143};
uint8 t4[] = {90, 45, 152, 19, 38, 76};
uint8 t5[] = {104, 101, 108, 108, 111, 3};

uint8 t1s[] = {104, 101, 108, 110, 109, 3, 0};
uint8 t2s[] = {17, 9, 5, 143, 3, 1};
uint8 t3s[] = {9, 5, 3, 70, 143, 2};
uint8 t4s[] = {90, 45, 152, 19, 38, 76, 3};
uint8 t5s[] = {104, 101, 108, 108, 111, 3, 4};


struct test_case {
    uint8* poly;
    size_t len;
};

test_case tests[13];
test_case tests_s[13];


void poly_handler_test(){
    srand(clock());

    PolyHandler PH(16, 8, INIT_ALL);

    ALLOC_MEMORY(PH);
    ALLOC_POLY(PH);

    PH.init();

    for(int i = 0; i < PH.poly_count; i ++){
        int len = rand() % 9;
        tests[i].poly = (uint8*) malloc(len * sizeof(uint8));
        tests[i].len = len;
        for(int j = 0; j < len; j++)
            tests[i].poly[j] = rand() % 256;
    }

    for(int i = 0; i < PH.poly_count; i ++){
        int len = rand() % 9;
        tests[i].poly = (uint8*) malloc(len * sizeof(uint8));
        tests[i].len = len;
        for(int j = 0; j < len; j++)
            tests[i].poly[j] = rand() % 256;
        tests[i].poly[len-1] = i;
    }


    std::cout << "Testing init_poly:\t\t\t" << ((init_poly_test(PH)) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing set_poly():\t\t\t" << ((set_poly_test(PH)) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing append():\t\t\t" << ((stretch_poly_test(PH)) ? "SUCCESS" : "FAILURE") << std::endl;

}


bool init_poly_test(PolyHandler &PH){
    for(uint i = 0; i < PH.poly_count; i++){
        PH.reset_poly(i);
        if(PH[i].len != 0 || PH[i].size != PH.poly_size) return false;
    }
    return true;
}

bool set_poly_test(PolyHandler &PH) {
    for(uint i = 0; i < PH.poly_count; i++){
        PH.set_poly(i, tests[i].poly, tests[i].len);
        if(PH[i].len != tests[i].len) return false;
        if(memcmp(PH[i].ptr, tests[i].poly, tests[i].len * sizeof(uint8)) != 0) return false;
    }
    return true;
}

bool stretch_poly_test(PolyHandler &PH) {
    for(uint i = 0; i < PH.poly_count; i++){
        bool ok = PH[i].append(i);

        if((PH[i].len+1) * sizeof(uint8) > PH[i].size){
            if(ok) return false;
            else   return true;
        }

        if(memcmp(PH[i].ptr, tests[i].poly, tests[i].len * sizeof(uint8)) != 0) return false;
    }
    return true;
}

}


*/
