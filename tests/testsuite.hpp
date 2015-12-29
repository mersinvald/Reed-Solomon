/* Author: Mike Lubinets (aka mersinvald)
 * Date: 29.12.15
 *
 * See LICENSE */

#ifndef TESTSUITE_H
#define TESTSUITE_H
#include <iostream>
#include <string.h>

class TestSuite {
public:
    virtual bool run_tests() = 0;

    template<typename T>
    static bool compare(T var, T ans) {
        if(var != ans) {
            std::cout << "expected: " << ans;
            std::cout << "got:      " << var;
            return false;
        }
        return true;
    }

protected:

    template<typename T>
    static bool compare(T *var, T *ans, uint varlen, uint anslen) {
        if(varlen != anslen) {
            std::cout << "expected length " << anslen << ", "
                      << "got " << varlen << std::endl;
            return false;
        }

        if(memcmp(var, ans, varlen * sizeof(T)) != 0) {
            std::cout << "expected: "; print_array(ans, anslen);
            std::cout << "got:      "; print_array(var, varlen);
            return false;
        }

        return true;
    }

    static bool test(bool (*fptr)(std::string&)) {
        std::string name;
        bool ok = fptr(name);
        std::cout << name << ": " << (ok ? "SUCCESS" : "FAILURE") << std::endl;
        return ok;
    }

private:
    template<typename T>
    static void print_array(T *array, uint length) {
        for(uint i = 0; i < length; i++) {
            std::cout << array[i];
        }
        std::cout << std::endl;
    }
};

#endif // TESTSUITE_H
