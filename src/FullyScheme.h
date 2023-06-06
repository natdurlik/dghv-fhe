#ifndef DGHV_FHE_FULLYSCHEME_H
#define DGHV_FHE_FULLYSCHEME_H

#include "SomewhatScheme.h"
#include "PublicKey.h"

class FullyScheme : private SomewhatScheme {
public:
    int theta;
    int Theta;
    int kappa;

    FullyScheme(int security, long rd_seed) : SomewhatScheme(security, rd_seed) {
        theta = lambda;
        kappa = gamma * eta / ro_prim;
        Theta = kappa;
    }

    std::pair<std::vector<NTL::GF2>, PublicKey> key_gen();

    mpz_class encrypt(const std::vector<mpz_class> &pk, NTL::GF2 message);

    std::vector<std::vector<NTL::GF2>> post_process(mpz_class c);

    NTL::GF2 decrypt(mpz_class sk, mpz_class c);

    template<typename T>
    T recrypt(std::vector<T> c, std::vector<T> s, std::vector<std::vector<T>> z);

    template<typename T>
    std::vector<T> hamming_weight(std::vector<T> a);

};


#endif //DGHV_FHE_FULLYSCHEME_H
