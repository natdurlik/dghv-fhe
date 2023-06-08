#include <gtest/gtest.h>
#include "FullyScheme.h"

TEST(HammingWeightBinary, AllZeros) {
    FullyScheme fhe = FullyScheme(8, 0);
    std::vector<NTL::GF2> a = {NTL::GF2{0}};
    auto out = fhe.hamming_weight(a);
    for (auto x: out) {
        EXPECT_EQ(x, NTL::GF2{0});
    }

    a = {NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}};
    out = fhe.hamming_weight(a);
    for (auto x: out) {
        EXPECT_EQ(x, NTL::GF2{0});
    }

    a = {NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}};
    out = fhe.hamming_weight(a);
    for (auto x: out) {
        EXPECT_EQ(x, NTL::GF2{0});
    }
}


TEST(HammingWeightBinary, ZerosAndOne) {
    FullyScheme fhe = FullyScheme(8, 0);
    std::vector<NTL::GF2> a = {NTL::GF2{1}};
    auto out = fhe.hamming_weight(a);
    EXPECT_EQ(out[0], NTL::GF2{1});
    for (int i = 1; i < out.size(); i++) {
        EXPECT_EQ(out[i], NTL::GF2{0});
    }

    a = {NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}};
    out = fhe.hamming_weight(a);
    EXPECT_EQ(out[0], NTL::GF2{1});
    for (int i = 1; i < out.size(); i++) {
        EXPECT_EQ(out[i], NTL::GF2{0});
    }

    a = {NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{1}};
    out = fhe.hamming_weight(a);
    EXPECT_EQ(out[0], NTL::GF2{1});
    for (int i = 1; i < out.size(); i++) {
        EXPECT_EQ(out[i], NTL::GF2{0});
    }
}

TEST(HammingWeightBinary, SmallNumbers) {
    FullyScheme fhe = FullyScheme(8, 0);
    std::vector<NTL::GF2> a = {NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{1}};
    auto out = fhe.hamming_weight(a);
    // 3 in binary
    EXPECT_EQ(out[0], NTL::GF2{1});
    EXPECT_EQ(out[1], NTL::GF2{1});
    for (int i = 2; i < out.size(); i++) {
        EXPECT_EQ(out[i], NTL::GF2{0});
    }

    a = {NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{0}};
    out = fhe.hamming_weight(a);
    // 6 in binary
    EXPECT_EQ(out[0], NTL::GF2{0});
    EXPECT_EQ(out[1], NTL::GF2{1});
    EXPECT_EQ(out[2], NTL::GF2{1});
    for (int i = 3; i < out.size(); i++) {
        EXPECT_EQ(out[i], NTL::GF2{0});
    }

    a = {NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}};
    out = fhe.hamming_weight(a);
    // 8 in binary
    EXPECT_EQ(out[0], NTL::GF2{0});
    EXPECT_EQ(out[1], NTL::GF2{0});
    EXPECT_EQ(out[2], NTL::GF2{0});
    EXPECT_EQ(out[3], NTL::GF2{1});
    for (int i = 4; i < out.size(); i++) {
        EXPECT_EQ(out[i], NTL::GF2{0});
    }
}

TEST(SetBitsToOne, SimpleCheck) {
    FullyScheme fhe = FullyScheme(8, 0);
    std::vector<NTL::GF2> s(fhe.Theta, NTL::GF2{0});
    fhe.set_theta_bits_to_one(s);
    unsigned long weight = 0;
    for (auto &x: s) {
        if (x == NTL::GF2{1}) {
            weight++;
        }
    }
    EXPECT_EQ(weight, fhe.theta);
}

TEST(FillWithSumToValue, SimpleCheck) {
    FullyScheme fhe = FullyScheme(8, 0);

    int Theta = 100;
    std::vector<NTL::GF2> s(Theta, NTL::GF2{0});
    s[1] = s[10] = s[32] = s[45] = s[98] = NTL::GF2{1};

    unsigned long kappa = 100;
    mpz_class sum_to;
    mpz_ui_pow_ui(sum_to.get_mpz_t(), 2, kappa);

    mpz_class range;
    mpz_ui_pow_ui(range.get_mpz_t(), 2, kappa + 1);

    std::vector<mpz_class> u(Theta);
    fhe.fill_with_random_integers_with_condition(u, s, range, sum_to);

    mpz_class sum = 0;
    for (int i = 0; i < u.size(); i++) {
        if (s[i] == NTL::GF2{1}) {
            sum += u[i];
        }
    }

    sum %= range;
    EXPECT_EQ(sum, sum_to);
}


