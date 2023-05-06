#include <iostream>
#include <gmpxx.h>

using namespace std;

mpz_class quot(mpz_class z, mpz_class p) {
    mpz_class rest = z % p;
    if (rest != 0 && rest >= p / 2) {
        return z / p + 1;
    }
    return z / p;
}

mpz_class rem(mpz_class z, mpz_class p) {
    return z - quot(z, p) * p;
}


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
