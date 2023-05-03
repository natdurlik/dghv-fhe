#include <iostream>
#include <gmpxx.h>

using namespace std;

int main() {
    std::cout << "Hello, World!" << std::endl;
//    mpz_t a;
    mpz_class a("3");
    mpz_class b("123112515151515");
    mpz_class res;
    res = a + b;
    cout << res << endl;
    return 0;
}
