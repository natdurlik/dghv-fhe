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

mpz_class rem(const mpz_class& z, const mpz_class& p) {
    return z - quot(z, p) * p;
}

bool is_even(const mpz_class& x) {
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
    if (x < 0) {
        integral = -integral;
        x = -x;
        x += integral % 2;
        return 2 - x;
    }

    x += integral % 2;
    return x;
}

std::vector<NTL::GF2> mpf_to_bits(mpf_class f, int n) {
    mp_exp_t exp;
    std::string bits_str = f.get_str(exp, 2);
    if (-exp + 1 < 0) {
        throw std::logic_error("expected rational smaller than 2");
    }
    unsigned start = -exp + 1;

    std::vector<NTL::GF2> bits_gf2(n, NTL::GF2{0});
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

mpf_class bits_to_mpf(const std::vector<NTL::GF2> &bits, int prec) {
    // expected rational in [0,2)
    if (bits.empty()) return 0;

    std::string bits_str;
    bool f = true;

    for (auto &gf2: bits) {
        if (IsOne(gf2)) {
            bits_str.push_back('1');
        } else {
            bits_str.push_back('0');
        }

        if (f) {
            bits_str.push_back('.');
            f = false;
        }
    }

    mpf_class ret(bits_str, prec, 2);
    return ret;
}

mpz_class round_to_closest(mpf_class x) { // only for tests, probably wrong in corner cases
    mpz_class integral(x);
    x -= integral;
    if (x < 0) {
        x = -x;
        if (x >= 0.5) {
            return integral - 1;
        }
        return integral;
    }

    if (x >= 0.5) {
        return integral + 1;
    }
    return integral;
}

std::vector<mpz_class> c_star_to_mpz(const std::vector<NTL::GF2> &bits) {
    std::vector<mpz_class> v(bits.size(), 0);
    for (int i = 0; i < bits.size(); i++) {
        if (IsOne(bits[i])) {
            v[i] = 1;
        }
    }
    return v;
}

std::vector<std::vector<mpz_class>> z_to_mpz(const std::vector<std::vector<NTL::GF2>> &bits) {
    std::vector<std::vector<mpz_class>> v(bits.size());
    for (int i = 0; i < bits.size(); i++) {
        v[i] = c_star_to_mpz(bits[i]);
    }
    return v;
}
