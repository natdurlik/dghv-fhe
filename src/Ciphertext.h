#ifndef DGHV_FHE_CIPHERTEXT_H
#define DGHV_FHE_CIPHERTEXT_H

#include <gmpxx.h>
#include <memory>

class Ciphertext : public mpz_class {
public:
    using mpz_class::mpz_class;

    std::shared_ptr<std::vector<mpz_class>> mod_red;

    mpz_class threshold;

    Ciphertext(const mpz_class &c, std::shared_ptr<std::vector<mpz_class>> reduction, const mpz_class &t);

    Ciphertext &operator+=(const Ciphertext &other);

    Ciphertext &operator*=(const Ciphertext &other);

    friend Ciphertext operator+(Ciphertext lhs, const Ciphertext &rhs);

    friend Ciphertext operator*(Ciphertext lhs, const Ciphertext &rhs);

    void reduce();
};

#endif //DGHV_FHE_CIPHERTEXT_H
