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

//    cout << "Scheme" << endl;

//    SomewhatScheme scheme(8, time(nullptr));

//    cout << scheme.gamma * scheme.eta / scheme.ro_prim << endl;
//    auto m1 = NTL::GF2(1);
//    auto m2 = NTL::GF2(1);
//    auto m3 = NTL::GF2(1);
//
//    mpz_class sk;
//    vector<mpz_class> pk;
//    tie(sk, pk) = scheme.key_gen();

//    cout << "pub key size= " << pk.size() << endl;
//    cout << "sk= " << sk << endl;
//    cout << is_even(sk) << endl;
//    cout << pk[0] << endl;
//    cout << "largest idx=" << distance(pk.begin(), max_element(pk.begin(), pk.end())) << endl;
//    cout << "pk even: " << is_even(pk[0]) << endl;
//    cout << "pk rest even: " << is_even(rem(pk[0], sk)) << endl;
//    cout << endl;
//
//    mpz_class c1 = scheme.encrypt(pk, m1);
//    mpz_class c2 = scheme.encrypt(pk, m2);
//    mpz_class c3 = scheme.encrypt(pk, m3);
//
//    mpz_class ev = add_circuit(c1, c2);
//    NTL::GF2 dec_m = scheme.decrypt(sk, ev);
//
//    cout << "decrypted " << dec_m << endl;
//    cout << "should be " << add_circuit(m1, m2) << endl;

//    cout<<" fully "<<endl;
//    FullyScheme fhe(8, time(nullptr));
//
//    mpz_class pow2;
//    mpz_ui_pow_ui(pow2.get_mpz_t(), 2, 100);
//
//    mpq_class q1(2,4);
//    mpq_class q3(2,pow2);
//    cout<<q1<<endl;
//    cout<<q3<<endl;
//    cout<<q1*q3<<endl;
//    cout<<q1<<endl;
//    q1.canonicalize();
//    cout<<q1<<endl;
//    cout<<q3.get_d()<<endl;
//
//    FullyScheme fhe(3,time(nullptr));
    FullyScheme fhe(8, time(nullptr));
    cout << "n=" << fhe.n << endl;
    cout << "Theta=" << fhe.Theta << endl;
//    cout << fhe.kappa << endl;
    vector<NTL::GF2> v{NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1},
                       NTL::GF2{0}};
    vector<NTL::GF2> weight = fhe.hamming_weight<>(v);
//    auto p = fhe.key_gen();
    for (auto x: weight) {
        cout << x << " ";
    }
    cout << endl;

    NTL::GF2 a1(1);
    NTL::GF2 a2(1);
    cout << a1 * a2 << endl;
    cout << int{1} << endl;
    return 0;
}
