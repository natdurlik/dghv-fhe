#include <iostream>
#include <gmpxx.h>
#include "src/rounded_quot_mod.h"

using namespace std;


int main() {
    cout << "Hello, World!" << endl;
//    mpz_t a;
    mpz_class a("7");
    mpz_class b("2");
    mpz_class res;
//    res = quot(a, b);
    cout << a / b << endl;
    cout << quot(a, b) << endl;
    cout << rem(a, b) << endl;
    return 0;
}
