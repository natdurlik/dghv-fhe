#include "rounded_quot_mod.h"

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
