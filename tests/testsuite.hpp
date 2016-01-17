#ifndef TESTSUITE_H
#define TESTSUITE_H
#include <iostream>
#include <string.h>

struct Report {
    uint32_t overall;
    uint32_t passed;
};

class TestSuite {
protected:
    template<typename T>
    static bool compare(T var, T ans) {
        if(var != ans) {
            std::cout << "expected: " << ans << std::endl;
            std::cout << "got:      " << var << std::endl;
            return false;
        }
        return true;
    }

    static bool compare(void *vvar, void *vans, size_t varlen, size_t anslen = 0) {
        char* var = (char*) vvar;
        char* ans = (char*) vans;
        if(!anslen) anslen = varlen;
        if(varlen != anslen) {
            std::cout << "expected length " << anslen << ", "
                      << "got " << varlen << std::endl;
            return false;
        }

        if(memcmp(var, ans, varlen) != 0) {
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

    // Поиск подстроки
    static bool has_substr(void* buffer, size_t bufsize, void* substr, size_t substrsize) {
    #ifdef __GNUG__ // If GNU extensions are present, use memmem
        return (memmem(buffer, bufsize, substr, substrsize) != NULL);
    #else           // Else do dirty trick with strings
        std::string haystick((char*)buffer, (char*)buffer+bufsize);
        std::string needle((char*)substr, (char*)substr+substrsize);
        return haystick.find(needle) != std::string::npos;
    #endif
    }

    #define INIT_TESTCASE               \
            name = __PRETTY_FUNCTION__;  \
            int  _stnum = 0;              \
            bool _ok = true;               \
            bool _temp;

    #define SUBTEST(expr) \
            _stnum++;      \
            _temp = (expr); \
            _ok &= _temp;    \
            if(!_temp) {      \
                std::cout << __FUNCTION__ << ":"                      \
                          << "SubTest #" << _stnum << " \"" #expr "\"" \
                          << " failed.\n";                              \
            }

    #define RETURN \
            return _ok;

private:
    static void print_array(char *array, uint length) {
        std::cout << std::hex;
        for(uint i = 0; i < length; i++) {
            std::cout << (int)array[i] << " ";
        }
        std::cout << std::endl;
    }
};

#endif // TESTSUITE_H
