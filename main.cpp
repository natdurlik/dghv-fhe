#include <iostream>
#include <iomanip>
#include <gmpxx.h>
#include "src/SomewhatScheme.h"
#include "src/FullyScheme.h"
#include <NTL/GF2.h>

using namespace std;

template<typename T>
T add_circuit(T m1, T m2) {
    return m1 + m2;
}

void print_prec(mpf_class x) {
    cout << "print_prec = " << x.get_prec() << endl;
}


int main() {

    FullyScheme fhe(6, 0);
    SecretKey secret_key;
    PublicKey public_key;
    std::tie(secret_key, public_key) = fhe.key_gen();

    cout << mpz_class{1} << endl;
    return 0;
}
