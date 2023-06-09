#include "FullyScheme.h"

FullyScheme::FullyScheme(int security, long rd_seed) : SomewhatScheme(security, rd_seed) {
    theta = lambda;
    kappa = gamma * eta / ro_prim;
    Theta = 20; // fixme
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
        if (s[i] == NTL::GF2{1}) {
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
        if (s[i] == NTL::GF2{1}) {
            m = 1;
        }

        mpz_class x = draw_from_distribution(q_range, r_range, p);
        mpz_class c = m + 2 * x;
        encrypted_s[i] = rem(c, pk[0]);;
    }

    return encrypted_s;
}

std::pair<std::vector<NTL::GF2>, PublicKey> FullyScheme::key_gen() {
    mpz_class sk;
    std::vector<mpz_class> pk;
    tie(sk, pk) = SomewhatScheme::key_gen();

    std::vector<NTL::GF2> s(Theta, NTL::GF2{0});
    set_theta_bits_to_one(s);

    mpz_class two_to_kappa = pow_of_two(kappa);
    mpz_class xp = two_to_kappa / sk; //fixme round

    mpz_class ui_range = pow_of_two(kappa + 1);

    std::vector<mpz_class> u(Theta);
    fill_with_random_integers_with_condition(u, s, ui_range, xp);

    // generate yi
    std::vector<mpf_class> y(Theta);
    for (int i = 0; i < y.size(); i++) {
        y[i] = mpf_class(u[i], kappa + 1);
        y[i] /= two_to_kappa;
    }

    std::vector<mpz_class> encrypted_sk = encrypt_secret_key_bits(s, sk, pk);

    PublicKey public_key{pk, y, encrypted_sk};

    return {s, public_key};
}

mpz_class FullyScheme::encrypt(const std::vector<mpz_class> &pk, NTL::GF2 message) {
    return SomewhatScheme::encrypt(pk, message);
}

std::vector<std::vector<NTL::GF2>>
FullyScheme::post_process(const std::vector<mpz_class> &c, const std::vector<mpf_class> &y) {

    return {};
}

NTL::GF2 FullyScheme::decrypt(mpz_class sk, mpz_class c) {
    return SomewhatScheme::decrypt(sk, c);
}

//template std::vector<int> FullyScheme::hamming_weight<int>(std::vector<int> a);

