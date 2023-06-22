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

    cout << std::setprecision(15);
//    cout << secret_key.p << endl;
//    cout << public_key.y.size() << endl;
//    cout << fhe.kappa << endl;
//    for (int i = 0; i < 5; i++) {
//        cout << public_key.y[i] << " ";
//    }
//    cout << endl;
    cout << "kappa=" << fhe.kappa << endl;
    mpf_class sum(0, fhe.kappa + 2);
    for (int i = 0; i < public_key.y.size(); i++) {
        if (IsOne(secret_key.s[i])) {
//            cout << "y: " << public_key.y[i] << ", ";
            sum += public_key.y[i];
//            cout << "sum: " << sum << "; ";
        }
    }
    cout << endl;
//    cout << sum << endl;
    mpf_class mod2sum = sum - 8;

    mpz_class two_to_kappa = pow_of_two(fhe.kappa);
//    cout << "two ^ kappa" << two_to_kappa << endl;
    mpz_class xp1 = quot(two_to_kappa, secret_key.p);
    mpf_class xpf(xp1, fhe.kappa + 2);
    std::cout << "xpf=" << xpf << std::endl;
    std::cout << "xpf/2^k=" << (xpf / two_to_kappa) << std::endl;

    mpf_class xpf2(two_to_kappa, fhe.kappa + 2);
    mpf_class pf(secret_key.p, fhe.kappa + 2);
//    xpf2 /= pf;
    xpf2 /= secret_key.p;
    cout << "xpf2=" << xpf2 << endl;

    mpf_class err(1, fhe.kappa + 2);
    err /= two_to_kappa;
    mpf_class one_over_p(1, fhe.kappa + 2);
    one_over_p /= pf;
    cout << "1/p = " << one_over_p << endl;
    cout << "err = " << err << endl;

    cout << "1/p-xp/2^k = " << one_over_p - (xpf / two_to_kappa) << endl;
    cout << "1/p-mod2sum = " << one_over_p - mod2sum << endl;
    mpf_class del = abs(one_over_p - mod2sum);
    cout << "del = " << del << endl;
    cout << "|sum-1/p|<err? = " << (del < err) << endl;
    cout << "|sum-1/p|-err = " << err - del << endl;
    cout << "1/p prec = " << one_over_p.get_prec() << endl;
    cout << "del prec = " << del.get_prec() << endl;
    cout << "xp/2^k prec = " << (xpf / two_to_kappa).get_prec() << endl;
    cout << "mod2sum prec = " << mod2sum.get_prec() << endl;
    cout << "err prec = " << err.get_prec() << endl;
    mpf_class tmp(err);
    print_prec(err);
    return 0;
}
