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

    FullyScheme(int security, int Theta, int tau, long rd_seed);

    std::pair<SecretKey, PublicKey> key_gen();

    mpz_class encrypt(const std::vector<mpz_class> &pk, NTL::GF2 message);

    std::pair<std::vector<NTL::GF2>, std::vector<std::vector<NTL::GF2>>>
    post_process(const mpz_class &c, const std::vector<mpf_class> &y);

    NTL::GF2 decrypt(const mpz_class &sk, const mpz_class &c);

    void set_theta_bits_to_one(std::vector<NTL::GF2> &s);

    void fill_with_random_integers_with_condition(std::vector<mpz_class> &u, const std::vector<NTL::GF2> &s,
                                                  const mpz_class &range, const mpz_class &sum_to);

    std::vector<mpz_class>
    encrypt_secret_key_bits(const std::vector<NTL::GF2> &s, const mpz_class &p, const std::vector<mpz_class> &pk);

    mpz_class recrypt(const mpz_class &c, const PublicKey &public_key);

    template<typename T>
    T squashed_decrypt(const std::vector<T> &c, const std::vector<T> &s, const std::vector<std::vector<T>> &z) {
        // assert s.size() == z.size()
        // set a_i = s[i] * z[i]
        std::vector<std::vector<T>> a = z;
        for (int i = 0; i < a.size(); i++) {
            for (int j = 0; j < a[i].size(); j++) {
                a[i][j] *= s[i];
            }
        }

        // generate w_j
        std::vector<std::vector<T>> w = generate_fewer_numbers(a);

        // three for two sum w_j
        auto [s1, s2] = k_for_two(w);

        // sum last two rationals with round mod 2
        T lsb = sum_round_mod2(s1, s2);

        return c.back() + lsb;
    }

    template<typename T>
    T or_t(const T &a, const T &b) {
        return (a + b) + (a * b);
    }

    template<typename T>
    T sum_round_mod2(const std::vector<T> &a, const std::vector<T> &b) {
        // assumes sum of a+b is within 1/4 of and integer
        T col = a[1] + b[1];
        T row = a[1] + a[2];
        return a[0] + b[0] + (or_t(col, row));
    }

    template<typename T>
    std::pair<std::vector<T>, std::vector<T>> k_for_two(const std::vector<std::vector<T>> &w) {
        if (w.size() < 2) {
            throw std::logic_error("expected at least two elements to add");
        }
        if (w.size() == 2) {
            return {w[0], w[1]};
        }

        std::vector<T> a = w[0];
        std::vector<T> b = w[1];

        for (int i = 2; i < w.size(); i++) {
            std::tie(a, b) = carry_save_adder_mod2(a, b, w[i]);
        }

        return {a, b};
    }

    template<typename T>
    std::pair<std::vector<T>, std::vector<T>>
    carry_save_adder_mod2(const std::vector<T> &a, const std::vector<T> &b, const std::vector<T> &c) {
        if (a.size() != b.size() || a.size() != c.size()) {
            throw std::logic_error("expected equal sizes of addends");
        }
        // a0 . a1 a2 a3 ...

        std::vector<T> sum(a.size());
        std::vector<T> carry(a.size(), T{});

        for (int i = 0; i < a.size(); i++) {
            sum[i] = a[i] + b[i] + c[i];
        }

        for (int i = 0; i + 1 < a.size(); i++) {
            carry[i] = a[i + 1] * b[i + 1] + a[i + 1] * c[i + 1] + b[i + 1] * c[i + 1];
        }

        return {sum, carry};
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
        std::vector<std::vector<T>> w = shift_and_mod2(W, n + 1); // fixme less than n bits of precision?

        return w;
    }

    template<typename T>
    std::vector<std::vector<T>> shift_and_mod2(const std::vector<std::vector<T>> &W, int prec) {

        std::vector<std::vector<T>> w(W.size(), std::vector<T>(prec, T{}));
        for (int j = 0; j < w.size(); j++) {
            int w_i = 0;
            int i = j;

            while (i >= W[j].size()) {
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

};


#endif //DGHV_FHE_FULLYSCHEME_H
