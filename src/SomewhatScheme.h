#ifndef DGHV_FHE_SOMEWHATSCHEME_H
#define DGHV_FHE_SOMEWHATSCHEME_H

#include <gmpxx.h>
#include <cmath>
#include "rounded_quot_mod.h"

class SomewhatScheme {
public:
    int lambda;
    int gamma;
    int eta;
    int ro;
    int ro_prim;
    int tau;
    int log2(int x);
    gmp_randclass rand;

    SomewhatScheme(int security, long rd_seed);
    SomewhatScheme(int lambda, int gamma, int eta, int ro, int ro_prim, int tau, int rd_seed);

    std::pair<mpz_class, std::vector<mpz_class>> key_gen(int);
    mpz_class encrypt(const std::vector<mpz_class> &pk, bool m);
    // evaluate is just a polynomial function, in easy version pk is not needed
    bool decrypt(mpz_class sk, mpz_class c);
    std::vector<mpz_class> sample_public_key(mpz_class p);
};


#endif //DGHV_FHE_SOMEWHATSCHEME_H
