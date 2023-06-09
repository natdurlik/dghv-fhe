#ifndef DGHV_FHE_ROUNDED_QUOT_MOD_H
#define DGHV_FHE_ROUNDED_QUOT_MOD_H

#include <gmpxx.h>

mpz_class quot(mpz_class z, mpz_class p);

mpz_class rem(mpz_class z, mpz_class p);

bool is_even(mpz_class x);

mpz_class pow_of_two(unsigned long exp);

#endif //DGHV_FHE_ROUNDED_QUOT_MOD_H
