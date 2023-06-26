#include "FullyScheme.h"
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

std::vector<NTL::GF2> mpf_to_bits(mpf_class f, int n) {
    mp_exp_t exp;
    std::string bits_str = f.get_str(exp, 2, n);
    if (-exp + 1 < 0) {
        throw std::logic_error("expected decimal smaller than 2");
    }
    unsigned start = -exp + 1;

    std::vector<NTL::GF2> bits_gf2(n + 1, NTL::GF2{0}); // n+1 because n bits of precision AFTER binary point
    for (auto gf2_idx = start, str_idx = 0u;
         gf2_idx < bits_gf2.size() && str_idx < bits_str.size(); gf2_idx++, str_idx++) {
        if (bits_str[str_idx] == '1') {
            bits_gf2[gf2_idx] = NTL::GF2{1};
        }
    }
    return bits_gf2;
}

std::vector<NTL::GF2> mpz_to_bits(mpz_class x) {
    std::string bits_str = x.get_str(2);
    std::vector<NTL::GF2> bits(bits_str.size(), NTL::GF2{0});
    for (int i = 0; i < bits_str.size(); i++) {
        if (bits_str[i] == '1') {
            bits[i] = NTL::GF2{1};
        }
    }

    return bits;
}