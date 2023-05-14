#ifndef DGHV_FHE_SOMEWHATSCHEME_H
#define DGHV_FHE_SOMEWHATSCHEME_H

#include <gmpxx.h>
#include <cmath>
#include "rounded_quot_mod.h"
#include <iostream>

class SomewhatScheme {
public:
    int lambda;
    int gamma;
    int eta;
    int ro;
    int ro_prim;
    int tau;
    gmp_randclass rand;

    SomewhatScheme(int security, long rd_seed);
    SomewhatScheme(int lambda, int gamma, int eta, int ro, int ro_prim, int tau, int rd_seed);

    std::pair<mpz_class, std::vector<mpz_class>> key_gen();
    mpz_class encrypt(const std::vector<mpz_class> &pk, bool m);
    bool decrypt(mpz_class sk, mpz_class c);
    std::vector<mpz_class> sample_public_key(mpz_class p);
    mpz_class sample_secret_key();
    int log2(int x);
};


#endif //DGHV_FHE_SOMEWHATSCHEME_H
