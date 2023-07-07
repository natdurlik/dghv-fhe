#ifndef DGHV_FHE_SOMEWHATSCHEME_H
#define DGHV_FHE_SOMEWHATSCHEME_H

#include <gmpxx.h>
#include <cmath>
#include "utils.h"
#include "Ciphertext.h"
#include "PublicKey.h"
#include <iostream>
#include <NTL/GF2.h>

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

    std::pair<mpz_class, PublicKey> key_gen();

    Ciphertext encrypt(const PublicKey &public_key, NTL::GF2 message);

    NTL::GF2 decrypt(const mpz_class& sk, const mpz_class& c);

    std::vector<mpz_class> sample_public_key(const mpz_class& p);

    mpz_class sample_secret_key();

    mpz_class draw_from_distribution(const mpz_class& q_range, const mpz_class& r_range, const mpz_class& p);

    double log2(int x);
};


#endif //DGHV_FHE_SOMEWHATSCHEME_H
