#include "FullyScheme.h"

FullyScheme::FullyScheme(int security, long rd_seed) : SomewhatScheme(security, rd_seed) {
    theta = lambda;
    kappa = gamma * eta / ro_prim;
    Theta = 20; // fixme
    n = std::ceil(log2(theta)) + 3;
}

FullyScheme::FullyScheme(int security, int Theta, long rd_seed) : SomewhatScheme(security, rd_seed) {
    theta = lambda;
    kappa = gamma * eta / ro_prim;
    this->Theta = Theta;
    n = std::ceil(log2(theta)) + 3;
}

std::pair<std::vector<NTL::GF2>, PublicKey> FullyScheme::key_gen() {
    return {};
}

mpz_class FullyScheme::encrypt(const std::vector<mpz_class> &pk, NTL::GF2 message) {
    return SomewhatScheme::encrypt(pk, message);
}

std::vector<std::vector<NTL::GF2>> FullyScheme::post_process(mpz_class c) {
    return {};
}

NTL::GF2 FullyScheme::decrypt(mpz_class sk, mpz_class c) {
    return SomewhatScheme::decrypt(sk, c);
}

//template std::vector<int> FullyScheme::hamming_weight<int>(std::vector<int> a);

