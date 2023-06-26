#include "utils.h"

mpz_class quot(mpz_class z, mpz_class p) {
    bool minus = false;
    if (z < 0) {
        z = -z;
        minus ^= true;
    }
    if (p < 0) {
        p = -p;
        minus ^= true;
    }

    if (minus) {
        mpz_class rest = z % p;
        if (rest != 0 && rest > p / 2) {
            return -z / p - 1;
        }
        return -z / p;
    }

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

mpz_class pow_of_two(unsigned long exp) {
    mpz_class x;
    mpz_ui_pow_ui(x.get_mpz_t(), 2, exp);
    return x;
}

mpf_class mod2f(mpf_class x) {
    mpz_class integral(x);
    x -= integral;
    if (integral % 2 == 1) {
        x += 1;
    }
    return x;
}
