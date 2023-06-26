#ifndef DGHV_FHE_FULLYSCHEME_H
#define DGHV_FHE_FULLYSCHEME_H

#include "SomewhatScheme.h"
#include "PublicKey.h"
#include "SecretKey.h"

class FullyScheme : public SomewhatScheme {
public:
    int theta;
    int Theta;
    int kappa;
    int n;

    FullyScheme(int security, long rd_seed);

    FullyScheme(int security, int Theta, long rd_seed);

    std::pair<SecretKey, PublicKey> key_gen();

    mpz_class encrypt(const std::vector<mpz_class> &pk, NTL::GF2 message);

    std::pair<std::vector<NTL::GF2>, std::vector<std::vector<NTL::GF2>>>
    post_process(mpz_class c, const std::vector<mpf_class> &y);

    NTL::GF2 decrypt(mpz_class sk, mpz_class c);

    void set_theta_bits_to_one(std::vector<NTL::GF2> &s);

    void fill_with_random_integers_with_condition(std::vector<mpz_class> &u, const std::vector<NTL::GF2> &s,
                                                  mpz_class range, mpz_class sum_to);

    std::vector<mpz_class>
    encrypt_secret_key_bits(const std::vector<NTL::GF2> &s, mpz_class p, const std::vector<mpz_class> &pk);

    template<typename T>
    T recrypt(std::vector<T> c, std::vector<T> s, std::vector<std::vector<T>> z) {

        // assert s.size() == z.size()
        // set a_i = s[i] * z[i]
        std::vector<std::vector<T>> a = z;
        for (int i = 0; i < a.size(); i++) {
            for (int j = 0; j < a[i].size(); j++) {
                a[i] *= s[i];
            }
        }

        // generate w_j
        std::vector<std::vector<T>> w = generate_fewer_numbers(a);

        // three for two sum w_j

        // sum last two rationals with round mod 2

        return c.back();
    }

    template<typename T>
    std::vector<std::vector<T>> generate_fewer_numbers(const std::vector<std::vector<T>> &a) {
        // get columns
        std::vector<std::vector<T>> cols(a[0].size(), std::vector<T>(a.size()));
        for (int i = 0; i < a.size(); i++) {
            for (int j = 0; j < a[i].size(); j++) {
                cols[j][i] = a[i][j];
            }
        }

        // compute W_{-j} - use hamming weight for each column
        std::vector<std::vector<T>> W(cols.size());
        for (int i = 0; i < cols.size(); i++) {
            W[i] = hamming_weight(cols[i]);
        }

        // w_j=(2^{-j}W_{-j}) shift 2^{-j} and mod2
        // w_j = w0.w1w2...
        std::vector<std::vector<T>> w(cols.size(), std::vector<T>(n + 1, T{}));
        for (int j = 0; j < w.size(); j++) {
            int w_i = 0;
            int i = j;

            while (i > W.size() && i >= 0 && w_i < w[j].size()) {
                i--;
                w_i++;
            }

            for (; i >= 0 && w_i < w.size(); i--, w_i++) {
                w[j][w_i] = W[j][i];
            }
        }

        return w;
    }

    template<typename T>
    std::vector<T> hamming_weight(const std::vector<T> &a) {
        int bits_length = std::ceil(log2(theta + 1));
        std::vector<T> W(bits_length, T{});
        unsigned pol_size = std::ceil(std::pow(2, bits_length - 1));
        std::vector<std::vector<T>> dp(pol_size + 1, std::vector<T>(a.size() + 1, T{}));

        for (int k = 0; k <= a.size(); k++) {
            dp[0][k] = T{1};
        }

        for (int k = 1; k <= a.size(); k++) {
            for (int j = 1; j <= pol_size; j++) {
                dp[j][k] = a[k - 1] * dp[j - 1][k - 1] + dp[j][k - 1];
            }
        }

        unsigned pow2 = 1;
        for (int i = 0; i < bits_length; i++) {
            W[i] = dp[pow2][a.size()];
            pow2 *= 2;
        }

        return W;
    }

    std::vector<NTL::GF2> mpf_to_bits(mpf_class f);

    std::vector<NTL::GF2> mpz_to_bits(mpz_class x);
};


#endif //DGHV_FHE_FULLYSCHEME_H
