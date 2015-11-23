#include <iostream>
#include <cstring>
#include "gf.hpp"
#include "rs.hpp"

using namespace std;

int main()
{

    //gf::init_tables();
    gf::test::run_tests();
    RS::test::run_tests();

    uint msgsize = 16;
    uint8 msg[16] = {0x40, 0xd2, 0x75, 0x47, 0x76, 0x17, 0x32, 0x06, 0x27, 0x26, 0x96, 0xc6, 0xc6, 0x96, 0x70, 0xec };

    size_t outsize;
    uint8* out;

    cout << "\n\n";

    out = RS::encode_msg(msg, msgsize, 8, &outsize);

    cout << std::hex;

    for(uint i = 0; i < msgsize; i++)
        cout << (int)msg[i] << " ";

    cout << endl;

    for(uint i = 0; i < outsize; i++)
        cout << (int)out[i] << " ";

    cout << endl;

    return 0;
}

