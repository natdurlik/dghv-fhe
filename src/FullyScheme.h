#ifndef DGHV_FHE_FULLYSCHEME_H
#define DGHV_FHE_FULLYSCHEME_H

#include "SomewhatScheme.h"
#include "PublicKey.h"

class FullyScheme : private SomewhatScheme {
public:
    int theta;
    int Theta;
    int kappa;
    int n;

    FullyScheme(int security, long rd_seed);

    FullyScheme(int security, int Theta, long rd_seed);

    std::pair<std::vector<NTL::GF2>, PublicKey> key_gen();

    mpz_class encrypt(const std::vector<mpz_class> &pk, NTL::GF2 message);

    std::vector<std::vector<NTL::GF2>> post_process(mpz_class c);

    NTL::GF2 decrypt(mpz_class sk, mpz_class c);

    void set_theta_bits_to_one(std::vector<NTL::GF2> &s);

    void fill_with_random_integers_with_condition(std::vector<mpz_class> &u, const std::vector<NTL::GF2> &s,
                                                  mpz_class range, mpz_class sum_to);

    std::vector<mpz_class> encrypt_secret_key_bits(const std::vector<NTL::GF2> &s, const std::vector<mpz_class> &pk);

    template<typename T>
    T recrypt(std::vector<T> c, std::vector<T> s, std::vector<std::vector<T>> z) {
        return c.back();
    }

    template<typename T>
    std::vector<T> hamming_weight(const std::vector<T> &a) {
        std::vector<T> W(n + 1, T{});
        int pol_size = std::ceil(std::pow(2, n));
        std::vector<std::vector<T>> dp(pol_size + 1, std::vector<T>(a.size() + 1, T{}));

        for (int k = 0; k <= a.size(); k++) {
            dp[0][k] = T{1};
        }

        for (int k = 1; k <= a.size(); k++) {
            for (int j = 1; j <= pol_size; j++) {
                dp[j][k] = a[k - 1] * dp[j - 1][k - 1] + dp[j][k - 1];
            }
        }

        int pow2 = 1;
        for (int i = 0; i <= n; i++) {
            W[i] = dp[pow2][a.size()];
            pow2 *= 2;
        }

        return W;
    }

};


#endif //DGHV_FHE_FULLYSCHEME_H
