#include "SomewhatScheme.h"
#include "Ciphertext.h"
#include "PublicKey.h"

SomewhatScheme::SomewhatScheme(int security, long rd_seed) : rand(gmp_randinit_default) {
    lambda = security;
    ro = lambda;
    ro_prim = 2 * lambda;
    gamma = lambda * lambda * lambda * lambda * lambda;
    eta = lambda * lambda * log2(lambda);
    tau = gamma + lambda;

    rand.seed(rd_seed);
}

SomewhatScheme::SomewhatScheme(int lambda, int gamma, int eta, int ro, int ro_prim, int tau, int rd_seed) :
        lambda(lambda), gamma(gamma), eta(eta), ro(ro), ro_prim(ro_prim), tau(tau), rand(gmp_randinit_default) {
    rand.seed(rd_seed);
}

std::pair<mpz_class, PublicKey> SomewhatScheme::key_gen() {
    mpz_class sk = sample_secret_key();

    std::vector<mpz_class> pk = sample_public_key(sk);

    PublicKey public_key;
    public_key.pk = pk;

    mpz_class q_range = pow_of_two(gamma);
    q_range /= sk;
    mpz_class q = rand.get_z_range(q_range - 1);
    public_key.mod_red = sk * q;

    return {sk, public_key};
}

Ciphertext SomewhatScheme::encrypt(const PublicKey &public_key, NTL::GF2 message) {

    mpz_class m = 0;
    if (NTL::IsOne(message)) m = 1;

    // random subset S
    std::vector<mpz_class> s;
    for (int i = 1; i < public_key.pk.size(); i++) {
        mpz_class take = rand.get_z_bits(1);
        if (take == 1) {
            s.push_back(public_key.pk[i]);
        }
    }

    mpz_class r_range = pow_of_two(ro_prim);
    mpz_class r = rand.get_z_range(r_range - 1);
    mpz_class neg = rand.get_z_bits(1);
    if (neg == 1) {
        r = -r;
    }

    mpz_class c = m + 2 * r;

    for (const mpz_class &x: s) {
        c += (2 * x);
    }
    c = rem(c, public_key.pk[0]);

    Ciphertext ciphertext(c, public_key.mod_red);
    return ciphertext;
}

NTL::GF2 SomewhatScheme::decrypt(const mpz_class &sk, const mpz_class &c) {
    mpz_class m = rem(rem(c, sk), 2);
    if (m == 0) return NTL::GF2(0);
    return NTL::GF2(1);
}

mpz_class SomewhatScheme::sample_secret_key() {
    mpz_class a = pow_of_two(eta - 1); // 2^{eta-1}
    mpz_class b = rand.get_z_bits(eta - 1);
    while (is_even(b)) {
        b = rand.get_z_bits(eta - 1);
    }
    mpz_class sk = a + b;

    return sk;
}


std::vector<mpz_class> SomewhatScheme::sample_public_key(const mpz_class &p) {
    mpz_class q_range = pow_of_two(gamma);
    q_range /= p;

    mpz_class r_range = pow_of_two(ro);

    std::vector<mpz_class> x(tau + 1);
    x[0] = 2;

    while (is_even(x[0]) || !is_even(rem(x[0], p))) {
        for (int i = 0; i < tau + 1; i++) {
            x[i] = draw_from_distribution(q_range, r_range, p);
        }

        unsigned long max_idx = distance(x.begin(), std::max_element(x.begin(), x.end()));
        swap(x[0], x[max_idx]);
    }

    return x;
}

mpz_class
SomewhatScheme::draw_from_distribution(const mpz_class &q_range, const mpz_class &r_range, const mpz_class &p) {
    mpz_class q = rand.get_z_range(q_range - 1);
    mpz_class r = rand.get_z_range(r_range - 1);
    mpz_class neg = rand.get_z_bits(1);
    if (neg == 0) {
        r = -r;
    }
    return p * q + r;
}

