#ifndef DGHV_FHE_ROUNDED_QUOT_MOD_H
#define DGHV_FHE_ROUNDED_QUOT_MOD_H

#include <gmpxx.h>

mpz_class quot(mpz_class z, mpz_class p) {
    mpz_class rest = z % p;
    if (rest != 0 && rest >= p / 2) {
        return z / p + 1;
    }
    return z / p;
}

mpz_class rem(mpz_class z, mpz_class p) {
    return z - quot(z, p) * p;
}

bool is_even(mpz_class x) {
    return rem(x, 2) == 0;
}

#endif //DGHV_FHE_ROUNDED_QUOT_MOD_H
