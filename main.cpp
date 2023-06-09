#include <iostream>
#include <gmpxx.h>
#include "src/SomewhatScheme.h"
#include "src/FullyScheme.h"
#include <NTL/GF2.h>

using namespace std;

template<typename T>
T add_circuit(T m1, T m2) {
    return m1 + m2;
}

int main() {
//
////    FullyScheme fhe(3,time(nullptr));
//    FullyScheme fhe(8, time(nullptr));
//    cout << "n=" << fhe.n << endl;
//    cout << "Theta=" << fhe.Theta << endl;
////    cout << fhe.kappa << endl;
//    vector<NTL::GF2> v{NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1},
//                       NTL::GF2{0}};
//    vector<NTL::GF2> weight = fhe.hamming_weight<>(v);
////    auto p = fhe.key_gen();
//    for (auto x: weight) {
//        cout << x << " ";
//    }
//    cout << endl;
//
//    NTL::GF2 a1(1);
//    NTL::GF2 a2(1);
//    cout << a1 * a2 << endl;
//    cout << int{1} << endl;

    mpz_class x = 10000;
    mpz_class pow;
    mpz_ui_pow_ui(pow.get_mpz_t(), 2, 10);

    mpf_class f(x, 5);
    f /= pow;
    cout << f << endl;
//    cout << (f % 2) << endl;
    mp_exp_t exp;
    cout << f.get_str(exp, 10, 10) << endl;
    cout << exp << endl;
    cout << f.get_str(exp, 2, 50) << endl;
    cout << exp << endl;
//    cout << "q" << endl;
//    mpq_class q = mpq_class(x, pow);
//    cout << q << endl;
//    cout << q.get_str(2) << endl;

    return 0;
}
