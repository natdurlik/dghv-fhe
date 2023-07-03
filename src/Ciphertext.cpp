#include "Ciphertext.h"
#include "utils.h"

Ciphertext::Ciphertext(const mpz_class &c, std::shared_ptr<std::vector<mpz_class>> reduction, const mpz_class &t)
        : mpz_class(c) {
    mod_red = reduction;
    threshold = t;
}

Ciphertext &Ciphertext::operator+=(const Ciphertext &other) {
    mpz_class::operator+=(other);
    reduce();
    return *this;
}

Ciphertext &Ciphertext::operator*=(const Ciphertext &other) {
    mpz_class::operator*=(other);
    reduce();
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

void Ciphertext::reduce() {
    if (abs(*this) > threshold) {
        std::cout << "reduction" << std::endl;
        for (int i = (int) mod_red->size() - 1; i >= 0; i--) {
            mpz_class::operator=(rem(*this, (*mod_red)[i]));
        }
    }
}
