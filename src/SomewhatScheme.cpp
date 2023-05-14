#include "SomewhatScheme.h"

SomewhatScheme::SomewhatScheme(int security, long rd_seed) : rand(gmp_randinit_default) {
    lambda = security;
    ro = lambda;
    ro_prim = 2 * lambda;
    gamma = lambda * lambda * lambda * lambda * lambda;
    eta = lambda * lambda;
    tau = gamma + lambda;

    rand.seed(rd_seed);
}

SomewhatScheme::SomewhatScheme(int lambda, int gamma, int eta, int ro, int ro_prim, int tau, int rd_seed) :
        lambda(lambda), gamma(gamma), eta(eta), ro(ro), ro_prim(ro_prim), tau(tau), rand(gmp_randinit_default) {
    rand.seed(rd_seed);
}

std::pair<mpz_class, std::vector<mpz_class>> SomewhatScheme::key_gen() {
    mpz_class sk = sample_secret_key();

    std::vector<mpz_class> pk = sample_public_key(sk);

    return {sk, pk};
}

mpz_class SomewhatScheme::encrypt(const std::vector<mpz_class> &pk, bool message) {
    mpz_class m = message;

    // random subset S
    std::vector<mpz_class> s;
    for (int i = 1; i < pk.size(); i++) {
        mpz_class take = rand.get_z_bits(1);
        if (take == 1) {
            s.push_back(pk[i]);
        }
    }
    std::cout << "encrypt subset size= " << s.size() << std::endl;

    mpz_class r_range;
    mpz_ui_pow_ui(r_range.get_mpz_t(), 2, ro_prim);
    mpz_class r = rand.get_z_range(r_range - 1);
    mpz_class neg = rand.get_z_bits(1);
    if (neg == 1) {
        r = -r;
    }

    mpz_class c = m + 2 * r;

//    std::cout << "m=" << message << ", c decrypt =" << rem(rem(c, 315), 2) << std::endl;
    for (const mpz_class &x: s) {
        c += (2 * x);
//        std::cout << "m=" << message << ", c decrypt =" << rem(c, 315) << std::endl;
    }
    c = rem(c, pk[0]);

    return c;
}

bool SomewhatScheme::decrypt(mpz_class sk, mpz_class c) {
    mpz_class m = rem(rem(c, sk), 2);
//    std::cout << m << std::endl;
    return m != 0;
}

mpz_class SomewhatScheme::sample_secret_key() {
    mpz_class a = 0;
    mpz_ui_pow_ui(a.get_mpz_t(), 2, eta - 1); // 2^{eta-1}
    mpz_class b = rand.get_z_bits(eta - 1);
    while (is_even(b)) {
        b = rand.get_z_bits(eta - 1);
    }
    mpz_class sk = a + b;

    return sk;
}


std::vector<mpz_class> SomewhatScheme::sample_public_key(mpz_class p) {
    mpz_class q_range;
    mpz_ui_pow_ui(q_range.get_mpz_t(), 2, gamma);
    q_range /= p;

    mpz_class r_range;
    mpz_ui_pow_ui(r_range.get_mpz_t(), 2, ro);

    std::vector<mpz_class> x(tau + 1);
    x[0] = 2;
//
//    std::cout << "q: " << q_range << std::endl;
//    std::cout << "r: " << r_range << std::endl;
//    std::cout << "tau: " << tau << std::endl;
    int counter = 1;
    while (is_even(x[0]) || !is_even(rem(x[0], p))) {
//        std::cout << counter++ << std::endl;
//        std::cout<<x[0]<<std::endl;
        for (int i = 0; i < tau + 1; i++) {
            mpz_class q = rand.get_z_range(q_range - 1);
            mpz_class r = rand.get_z_range(r_range - 1);
            mpz_class neg = rand.get_z_bits(1);
            if (neg == 0) {
                r = -r;
            }

            x[i] = p * q + r;
        }

        long max_idx = distance(x.begin(), std::max_element(x.begin(), x.end()));
        swap(x[0], x[max_idx]);
    }

    return x;
}

int SomewhatScheme::log2(int x) {
    return std::log(x) / std::log(2);
}


