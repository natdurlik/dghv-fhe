#include "Ciphertext.h"
#include "utils.h"

Ciphertext::Ciphertext(const mpz_class &c, const mpz_class &reduction)
        : mpz_class(rem(c, reduction)) {
    mod_red = reduction;
}

Ciphertext &Ciphertext::operator+=(const Ciphertext &other) {
    mpz_class::operator+=(other);
    mpz_class::operator=(rem(*this, mod_red));
    return *this;
}

Ciphertext &Ciphertext::operator*=(const Ciphertext &other) {
    mpz_class::operator*=(other);
    mpz_class::operator=(rem(*this, mod_red));
    return *this;
}

Ciphertext operator+(Ciphertext lhs, const Ciphertext &rhs) {
    lhs += rhs;
    return lhs;
}

Ciphertext operator*(Ciphertext lhs, const Ciphertext &rhs) {
    lhs *= rhs;
    return lhs;
}
