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

vector<NTL::GF2> to_bits(mpf_class f, unsigned n) {
    vector<NTL::GF2> bits(n + 1, NTL::GF2{0}); // n+1 because n bits of precision AFTER binary point
    mp_exp_t exp;
    string bits_str = f.get_str(exp, 2, n);
    if (-exp + 1 < 0) {
        cout << "unexpected exp" << endl;
        return bits;
    }
    unsigned start = -exp + 1;

//    unsigned str_idx = 0u;
    for (auto i = start, str_idx = 0u; i < bits.size() && str_idx < bits_str.size(); i++, str_idx++) {
        if (bits_str[str_idx] == '1') {
            bits[i] = NTL::GF2{1};
        }
    }
    return bits;
}

int main() {

    FullyScheme fhe(6, 0);
    SecretKey secret_key;
    PublicKey public_key;
    std::tie(secret_key, public_key) = fhe.key_gen();
//    auto [sk, pk] = fhe.key_gen();
//
//    cout << "kappa = " << fhe.kappa << endl;
//    cout << "gamma = " << fhe.gamma / fhe.ro << endl;
//    cout << "n = " << fhe.n << endl;
//
//    mpf_class f(0.5, 100);
//    mp_exp_t exp = 1;
//    cout << f.get_str(exp, 2, fhe.n) << endl;
//    cout << "exp = " << exp << endl;
//    auto bits = mpf_to_bits(f, fhe.n);
//
//    cout << "GF2 bits" << endl;
//    for (auto x: bits) {
//        cout << x;
//    }
//    cout << endl;
    //
    mpf_class f (0,1);

    mpz_class x = 8;
    auto stri = x.get_str(2);
    cout << stri << endl;


    return 0;
}
