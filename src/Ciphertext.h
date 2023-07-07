#ifndef DGHV_FHE_CIPHERTEXT_H
#define DGHV_FHE_CIPHERTEXT_H

#include <gmpxx.h>
#include <memory>

class Ciphertext : public mpz_class {
public:
    using mpz_class::mpz_class;

    mpz_class mod_red;

    Ciphertext(const mpz_class &c, const mpz_class &reduction);

    Ciphertext &operator+=(const Ciphertext &other);

    Ciphertext &operator*=(const Ciphertext &other);

    friend Ciphertext operator+(Ciphertext lhs, const Ciphertext &rhs);

    friend Ciphertext operator*(Ciphertext lhs, const Ciphertext &rhs);

    void reduce();
};

#endif //DGHV_FHE_CIPHERTEXT_H
