#ifndef DGHV_FHE_UTILS_H
#define DGHV_FHE_UTILS_H

#include <gmpxx.h>
#include <NTL/GF2.h>

mpz_class quot(mpz_class z, mpz_class p);

mpz_class rem(mpz_class z, mpz_class p);

bool is_even(mpz_class x);

mpz_class pow_of_two(unsigned long exp);

mpf_class mod2f(mpf_class x);

std::vector<NTL::GF2> mpf_to_bits(mpf_class f, int n);

std::vector<NTL::GF2> mpz_to_bits(mpz_class x);

mpf_class bits_to_mpf(const std::vector<NTL::GF2> &bits, int prec);

mpz_class round_to_closest(mpf_class x);

std::vector<mpz_class> c_star_to_mpz(const std::vector<NTL::GF2> &bits);

std::vector<std::vector<mpz_class>> z_to_mpz(const std::vector<std::vector<NTL::GF2>> &bits);

#endif //DGHV_FHE_UTILS_H
