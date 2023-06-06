#include "FullyScheme.h"

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

template<typename T>
T FullyScheme::recrypt(std::vector<T> c, std::vector<T> s, std::vector<std::vector<T>> z) {
    return {};
}

template<typename T>
std::vector<T> FullyScheme::hamming_weight(std::vector<T> a) {
    return std::vector<T>();
}
