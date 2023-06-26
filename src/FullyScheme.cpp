#include "FullyScheme.h"

FullyScheme::FullyScheme(int security, long rd_seed) : SomewhatScheme(security, rd_seed) {
    theta = lambda;
//    kappa = gamma * eta / ro_prim;
    kappa = gamma + 2;
    Theta = 1000; // fixme
//    Theta = 20;
    n = std::ceil(log2(theta)) + 3;
}

FullyScheme::FullyScheme(int security, int Theta, long rd_seed) : SomewhatScheme(security, rd_seed) {
    theta = lambda;
    kappa = gamma * eta / ro_prim;
    this->Theta = Theta;
    n = std::ceil(log2(theta)) + 3;
}

void FullyScheme::set_theta_bits_to_one(std::vector<NTL::GF2> &s) {
    int weight = 0;
    while (weight < theta) {
        mpz_class random_idx = rand.get_z_range(s.size());
        unsigned long idx = random_idx.get_ui();
        if (s[idx] == NTL::GF2{0}) {
            weight++;
            s[idx] = NTL::GF2{1};
        }
    }
}

void
FullyScheme::fill_with_random_integers_with_condition(std::vector<mpz_class> &u, const std::vector<NTL::GF2> &s,
                                                      mpz_class range, mpz_class sum_to) {
    mpz_class sum = 0;
    unsigned long last_idx = 0;
    for (unsigned long i = 0; i < u.size(); i++) {
        u[i] = rand.get_z_range(range);
        if (IsOne(s[i])) {
            sum += u[i];
            last_idx = i;
        }
    }

    sum -= u[last_idx];
    sum %= range;
    mpz_class rest = sum_to - sum;
    if (rest < 0) {
        rest = range - sum + sum_to;
    }
    u[last_idx] = rest;

}

std::vector<mpz_class>
FullyScheme::encrypt_secret_key_bits(const std::vector<NTL::GF2> &s, mpz_class p, const std::vector<mpz_class> &pk) {
    std::vector<mpz_class> encrypted_s(s.size());

    // fixme ?
    mpz_class q_range = pow_of_two(gamma);
    q_range /= p;

    mpz_class r_range = pow_of_two(ro);

    for (int i = 0; i < encrypted_s.size(); i++) {
        mpz_class m = 0;
        if (IsOne(s[i])) {
            m = 1;
        }

        mpz_class x = draw_from_distribution(q_range, r_range, p);
        mpz_class c = m + 2 * x;
        encrypted_s[i] = rem(c, pk[0]);
    }

    return encrypted_s;
}

std::pair<SecretKey, PublicKey> FullyScheme::key_gen() {
    mpz_class sk;
    std::vector<mpz_class> pk;
    tie(sk, pk) = SomewhatScheme::key_gen();

    std::vector<NTL::GF2> s(Theta, NTL::GF2{0});
    set_theta_bits_to_one(s);

    mpz_class two_to_kappa = pow_of_two(kappa);
    mpz_class xp = quot(two_to_kappa, sk);
    mpf_class f_two_to_kappa(two_to_kappa, kappa + 2);
    mpf_class xpf(xp, kappa + 2);

    mpz_class ui_range = pow_of_two(kappa + 1);

    std::vector<mpz_class> u(Theta);
    fill_with_random_integers_with_condition(u, s, ui_range, xp);

    std::vector<mpf_class> y(Theta);
    for (int i = 0; i < y.size(); i++) {
        y[i] = mpf_class(u[i], kappa + 2);
        y[i] /= two_to_kappa;
    }

    std::vector<mpz_class> encrypted_sk = encrypt_secret_key_bits(s, sk, pk);

    SecretKey secret_key{sk, s};
    PublicKey public_key{pk, y, encrypted_sk};

    return {secret_key, public_key};
}

mpz_class FullyScheme::encrypt(const std::vector<mpz_class> &pk, NTL::GF2 message) {
    return SomewhatScheme::encrypt(pk, message);
}

std::pair<std::vector<NTL::GF2>, std::vector<std::vector<NTL::GF2>>>
FullyScheme::post_process(mpz_class c, const std::vector<mpf_class> &y) {
    std::vector<mpf_class> zf(y.size());

    for (int i = 0; i < zf.size(); i++) {
        zf[i] = mod2f(y[i] * c);
    }

    // z to bits
    std::vector<std::vector<NTL::GF2>> z(zf.size());
    for (int i = 0; i < z.size(); i++) {
        z[i] = mpf_to_bits(zf[i]);
    }

    auto c_bits = mpz_to_bits(c);

    return {c_bits, z};
}

NTL::GF2 FullyScheme::decrypt(mpz_class sk, mpz_class c) {
    return SomewhatScheme::decrypt(sk, c);
}

std::vector<NTL::GF2> FullyScheme::mpf_to_bits(mpf_class f) {
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

std::vector<NTL::GF2> FullyScheme::mpz_to_bits(mpz_class x) {
    std::string bits_str = x.get_str(2);
    std::vector<NTL::GF2> bits(bits_str.size(), NTL::GF2{0});
    for (int i = 0; i < bits_str.size(); i++) {
        if (bits_str[i] == '1') {
            bits[i] = NTL::GF2{1};
        }
    }

    return bits;
}


//template std::vector<int> FullyScheme::hamming_weight<int>(std::vector<int> a);

