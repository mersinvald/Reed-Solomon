#include "rs.hpp"
#include "gf.hpp"
#include <memory.h>
#include <iostream>

namespace RS {
namespace test {

template<typename T>
void print_array(T *array, int count){
    for(int i = 0; i < count; i++){
        std::cout << (int)array[i] << " ";
    }
    std::cout << std::endl;
}

void print_poly(Poly &p){
    for(int i = 0; i < p.len; i++){
        std::cout << (int)p[i] << " ";
    }
    std::cout << std::endl;
}

PolyHandler* PH;

void run_tests(){

    PolyHandler p(16, 8, INIT_ALL);

    ALLOC_MEMORY(p);
    ALLOC_POLY(p);

    p.init();
    PH = &p;

    std::cout << "Testing RS::generator_poly(): \t\t" << ((RS::test::generator_poly_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing RS::encode_msg(): \t\t" << ((RS::test::encode_msg_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "\n";

    std::cout << "Testing RS::calc_syndromes(): \t\t" << ((RS::test::calc_syndromes_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing RS::find_errata_locator(): \t" << ((RS::test::find_errata_locator_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing RS::find_error_evaluator(): \t" << ((RS::test::find_error_evaluator_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing RS::correct_errata(): \t\t" << ((RS::test::correct_errata_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing RS::find_error_locator(): \t" << ((RS::test::find_error_locator_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    //std::cout << "Testing RS::find_errors(): \t\t" << ((RS::test::find_errors_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing RS::forney_syndromes(): \t" << ((RS::test::forney_syndromes_test()) ? "SUCCESS" : "FAILURE") << std::endl;
    std::cout << "Testing RS::correct_msg(): \t\t" << ((RS::test::correct_msg_test()) ? "SUCCESS" : "FAILURE") << std::endl;

}

bool generator_poly_test(){
    int nsym = 8;

    uint8 right_gp[9] = {1, 255, 11, 81, 54, 239, 173, 200, 24};

    uint8 gpn  = GENERATOR;
    uint8 rgpn = T_POLY3;

    Poly &rgp = PH->poly(rgpn);
    Poly &gp  = PH->poly(gpn);

    PH->set_poly(rgpn, right_gp, 9);

    generator_poly(nsym, gp, *PH);

    if(gp == rgp) return true;

    std::cout << "generator_poly():\nexpected:\t";
    print_poly(rgp);
    std::cout << "\ngot:\t\t";
    print_poly(gp);
    std::cout << std::endl;
    return false;
}

bool encode_msg_test(){
    uint8 in_msg[16] = {0x40, 0xd2, 0x75, 0x47, 0x76, 0x17, 0x32, 0x06, 0x27, 0x26, 0x96, 0xc6, 0xc6, 0x96, 0x70, 0xec};
    uint8 right_out_msg[24] = {64, 210, 117, 71, 118, 23, 50, 6, 39, 38, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    uint8 out_msg[24];

    encode_msg(in_msg, sizeof(in_msg), out_msg, sizeof(out_msg), 8);

    if(memcmp(right_out_msg, out_msg, sizeof(out_msg)) == 0) return true;

    std::cout << "encode_msg():\nexpected:\t";
    print_array(right_out_msg, 24);
    std::cout << "\ngot:\t\t";
    print_array(out_msg, 24);
    std::cout << std::endl;
    return false;
}

bool calc_syndromes_test(){
    uint8 enc_msg[24] = {64, 210, 117, 71, 118, 23, 50, 6, 39, 38, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    uint8 e_r_synd[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    uint8 cor_msg[24] = {64, 15, 22, 71, 118, 23, 50, 13, 39, 88, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    uint8 c_r_synd[9] = {0, 203, 98, 254, 12, 227, 127, 129, 190};

    uint8 emsgn = MSG_IN;
    uint8 cmsgn = MSG_OUT;
    uint8 ersyndn = T_POLY1;
    uint8 crsyndn = T_POLY2;
    uint8 esyndn   = FORNEY;
    uint8 csyndn   = SYNDROMES;

    Poly &emsg = PH->msg(emsgn);
    Poly &cmsg = PH->msg(cmsgn);
    Poly &ersynd = PH->poly(ersyndn);
    Poly &crsynd = PH->poly(crsyndn);
    Poly &esynd  = PH->poly(esyndn);
    Poly &csynd  = PH->poly(csyndn);

    PH->set_msg(emsgn, enc_msg, 24);
    PH->set_msg(cmsgn, cor_msg, 24);
    PH->set_poly(ersyndn, e_r_synd, 9);
    PH->set_poly(crsyndn, c_r_synd, 9);

    calc_syndromes(emsg, 8, esynd);
    calc_syndromes(cmsg, 8, csynd);

    bool clear_fail = false,
         corrupt_fail = false;

    if(ersynd != esynd) clear_fail   = true;
    if(crsynd != csynd) corrupt_fail = true;

    if(clear_fail) {
        std::cout << "calc_syndromes(): clear msg syndrome calculation failed\nexpected:\t";
        print_poly(ersynd);
        std::cout << "\ngot:\t\t";
        print_poly(esynd);
    }
    if(corrupt_fail) {
        std::cout << "calc_syndromes(): corrupted msg syndrome calculation failed\nexpected:\t";
        print_poly(crsynd);
        std::cout << "\ngot:\t\t";
        print_poly(csynd);
        std::cout << std::endl;
    }

    if(corrupt_fail || clear_fail) return false;
    return true;
}

bool find_errata_locator_test() {
    // uint8 err_msg[24] = {0, 210, 117, 71, 0, 23, 0, 6, 39, 38, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    // uint8 err_pos[3] = {0, 4, 6};
    uint8 coef_pos[3] = {23, 19, 17};
    uint8 right_eloc[4] = {210, 250, 11, 1};

    Poly &cpos  = PH->poly(COEF_POS);
    Poly &reloc = PH->poly(SYNDROMES);
    Poly &eloc  = PH->poly(ERASURES_LOC);

    PH->set_poly(COEF_POS, coef_pos, 3);
    PH->set_poly(SYNDROMES, right_eloc, 4);

    find_errata_locator(cpos, eloc, *PH);

    if(reloc == eloc) return true;

    std::cout << "find_errata_locator():\n"
                 "expected:\t";
    print_poly(reloc);
    std::cout << "\ngot:\t\t";
    print_poly(eloc);
    return false;
}

bool find_error_evaluator_test() {
    uint8 err_msg[24] = {0, 210, 117, 71, 0, 23, 0, 6, 39, 38, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    uint8 right_reeval[4] = {0, 175, 42, 4};
    uint8 coef_pos[3] = {23, 19, 17};

    Poly &emsg = PH->msg(MSG_IN);

    Poly &rreeval = PH->poly(ERRORS);
    Poly &reeval  = PH->poly(ERR_EVAL);

    Poly &coefs  = PH->poly(COEF_POS);
    Poly &synd   = PH->poly(SYNDROMES);
    Poly &rsynd  = PH->poly(T_POLY3);
    Poly &eloc   = PH->poly(ERASURES_LOC);

    PH->set_msg(MSG_IN, err_msg, 24);
    PH->set_poly(ERRORS, right_reeval, 4);
    PH->set_poly(COEF_POS, coef_pos, 3);

    calc_syndromes(emsg, 8, synd);
    find_errata_locator(coefs, eloc, *PH);

    /* Some weird shit happens when reverting with right for-loop
     * for(int i = synd.len-1, j = 0; i >= 0; i--, j++)
     * BUG FUCKING ATTENTION */
    for(int i = synd.len, j = 0; i >= 0; i--, j++) {
        rsynd[j] = synd[i];
    }

    rsynd.len = synd.len;

    find_error_evaluator(rsynd, eloc, eloc.len-1, reeval, *PH);

    if(rreeval == reeval) return true;

    std::cout << "find_error_evaluator():\n"
                 "expected:\t";
    print_poly(rreeval);
    std::cout << "\ngot:\t\t";
    print_poly(reeval);
    std::cout << "\n";

    return false;
}

bool correct_errata_test() {
    uint8 right_msg[24] = {64, 210, 117, 71, 118, 23, 50, 6, 39, 38, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    /* adding the erasure to test message */
    uint8 err_msg[24] = {0, 210, 117, 71, 0, 23, 0, 6, 39, 38, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    uint8 err_pos[3] = {0, 4, 6};

    Poly &msgin     = PH->msg(MSG_IN);
    Poly &corrected = PH->msg(MSG_OUT);
    Poly &synd = PH->poly(SYNDROMES);
    Poly &epos = PH->poly(ERASURES);

    PH->set_msg(MSG_IN, err_msg, 24);
    PH->set_poly(ERASURES, err_pos, 3);

    calc_syndromes(msgin, 8, synd);

    correct_errata(synd, epos, msgin, corrected, *PH);

    PH->set_msg(MSG_IN, right_msg, 24);
    if(msgin == corrected) return true;

    std::cout << "correct_errata():\n"
                 "expected:\t";
    print_poly(msgin);
    std::cout << "\ngot:\t\t";
    print_poly(corrected);
    std::cout << "\n";

    return false;
}

bool find_error_locator_test() {
    int   nsym = 8;
    uint8 case1[] = {0, 194, 192, 156, 139, 136, 252, 81, 95};
    uint8 case2[] = {0, 4, 6, 90, 77, 78, 58, 151, 153};
    uint8 case2_eloc[] = {0, 4};

    uint8 ans1[] = {210, 40, 241, 10, 1};
    uint8 ans2[] = {236, 176, 93, 4};

    Poly &synd1 = PH->poly(SYNDROMES);
    Poly &synd2 = PH->poly(FORNEY);
    Poly &eloc2 = PH->poly(ERASURES);

    Poly &ans1p = PH->poly(GENERATOR);
    Poly &ans2p = PH->poly(COEF_POS);

    Poly &errloc = PH->poly(ERRORS_LOC);

    PH->set_poly(SYNDROMES, case1, nsym+1);
    PH->set_poly(FORNEY, case2, nsym+1);
    PH->set_poly(ERASURES, case2_eloc, 2);
    PH->set_poly(GENERATOR, ans1, 5);
    PH->set_poly(COEF_POS, ans2, 4);

    find_error_locator(synd1, nsym, errloc, *PH);

    if(ans1p != errloc) {
        std::cout << "find_error_locator():\nexpected: ";
        print_poly(ans1p);
        std::cout << "\n got: ";
        print_poly(errloc);
        return false;
    }

    find_error_locator(synd2, nsym, errloc, *PH, &eloc2, eloc2.len);

    if(ans2p != errloc) {
        std::cout << "find_error_locator():\nexpected: ";
        print_poly(ans2p);
        std::cout << "\ngot: ";
        print_poly(errloc);
        return false;
    }
    return true;
}

bool find_errors_test() {
    /* weird shit but this function doesn't work even in example code O_O */
    uint8 right_msg[24] = {64, 210, 117, 71, 118, 23, 50, 6, 39, 38, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    uint8 err_msg[24]   = {64, 210, 117, 71, 118, 23, 50, 6, 39, 38, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    err_msg[0] = 6;

    Poly &msgout    = PH->msg(MSG_OUT);
    Poly &msgin     = PH->msg(MSG_IN);

    Poly &synd   = PH->poly(SYNDROMES);
    Poly &errloc = PH->poly(ERRORS_LOC);
    Poly &errors = PH->poly(ERRORS);

    PH->set_msg(MSG_IN, err_msg, 24);

    calc_syndromes(msgin, 8, synd);
    find_error_locator(synd, 8, errloc, *PH);
    find_errors(errloc, errors, 24);
    correct_errata(synd, errors, msgin, msgout, *PH);

    PH->set_msg(MSG_IN, right_msg, 24);

    if(msgin == msgout) return true;
    return false;
}

bool forney_syndromes_test() {
    uint8 err_msg[24]   = {0, 210, 0, 71, 6, 23, 50, 6, 39, 38, 150, 198, 198, 150, 112, 236, 232, 116, 20, 125, 112, 194, 194, 237};
    uint8 erasures[2]   = {0, 2};

    uint8 fsynd1[8] = {69, 192, 153, 116, 144, 165, 238, 127};
    uint8 fsynd2[8] = {169, 57, 246, 130, 178, 244, 81, 127};

    PH->set_msg(MSG_IN, err_msg, 24);
    PH->set_poly(T_POLY2, erasures, 2);
    PH->set_poly(T_POLY3, fsynd1, 8);
    PH->set_poly(T_POLY4, fsynd2, 8);

    Poly &synd  = PH->poly(SYNDROMES);
    Poly &fsynd = PH->poly(FORNEY);
    Poly &erpos = PH->poly(T_POLY2);

    Poly &right1 = PH->poly(T_POLY3);
    Poly &right2 = PH->poly(T_POLY4);

    calc_syndromes(PH->msg(MSG_IN), 8, synd);

    erpos.len = 0;
    forney_syndromes(synd, erpos, fsynd, 24, *PH);

    if(fsynd != right1) {
        std::cout << "expected: \t"; print_poly(right1);
        std::cout << "\ngot:\t\t";   print_poly(fsynd);
        return false;
    }

    erpos.len = 2;
    forney_syndromes(synd, erpos, fsynd, 24, *PH);

    if(fsynd != right2) {
        std::cout << "expected: \t"; print_poly(right2);
        std::cout << "\ngot:\t\t";   print_poly(fsynd);
        return false;
    }

    return true;

}

bool correct_msg_test() {
    srand(clock());

    int prim = 0x11d;
    int nsym = 8;

    gf::init_tables(prim);

    char msg[] = "Some VERY important message ought to be delivered safely";

    uint msg_size = sizeof(msg);
    uint msg_len  = sizeof(msg) / sizeof(char);

    uint8 encoded_msg[msg_len + nsym];
    uint encoded_msg_size = sizeof(encoded_msg);
    uint encoded_msg_len = encoded_msg_size / sizeof(uint8);

    encode_msg(msg, msg_size, encoded_msg, encoded_msg_size, nsym);

    for(uint i = 0; i < 4; i++) {
        encoded_msg[rand() % encoded_msg_len] += 3;
    }

    char repaired_msg[msg_len];
    correct_msg(encoded_msg, encoded_msg_size, repaired_msg, msg_size, nsym, NULL, 0);

    if(strcmp(msg, repaired_msg) == 0) return true;
    return false;
}

}
}
