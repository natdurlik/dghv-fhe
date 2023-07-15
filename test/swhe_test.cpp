#include <gtest/gtest.h>
#include "SomewhatScheme.h"

template<typename T>
T add_circuit(T m1, T m2) {
    return m1 + m2;
}

template<typename T>
T mul_circuit(T m1, T m2) {
    return m1 + m2;
}

template<typename T>
T circuit1(T m1, T m2, T m3) {
    return m1 + m2 * m3;
}

template<typename T>
T circuit2(T m1, T m2, T m3) {
    return m1 * m2 * m3;
}

template<typename T>
T circuit3(T m1, T m2, T m3, T m4, T m5, T m6) {
    return m1 + m2 * m3 + m4 * m5 + m6;
}

class SomewhatSchemeCircuitsTest : public ::testing::TestWithParam<long> {
};

INSTANTIATE_TEST_SUITE_P(RandomSeeds, SomewhatSchemeCircuitsTest, ::testing::Values(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10));

TEST_P(SomewhatSchemeCircuitsTest, AdditionZeroOne) {
    SomewhatScheme scheme(7, GetParam());
    auto [sk, pk] = scheme.key_gen();

    auto m1 = NTL::GF2(0);
    auto m2 = NTL::GF2(1);

    auto c1 = scheme.encrypt(pk, m1);
    auto c2 = scheme.encrypt(pk, m2);

    auto m1_out = add_circuit(m1, m2);
    auto c1_out = add_circuit(c1, c2);
    auto d1_out = scheme.decrypt(sk, c1_out);

    EXPECT_EQ(m1_out, d1_out);
}

TEST_P(SomewhatSchemeCircuitsTest, AdditionOnes) {
    SomewhatScheme scheme(7, GetParam());
    auto [sk, pk] = scheme.key_gen();

    auto m1 = NTL::GF2(1);
    auto m2 = NTL::GF2(1);

    auto c1 = scheme.encrypt(pk, m1);
    auto c2 = scheme.encrypt(pk, m2);

    auto m1_out = add_circuit(m1, m2);
    auto c1_out = add_circuit(c1, c2);
    auto d1_out = scheme.decrypt(sk, c1_out);

    EXPECT_EQ(m1_out, d1_out);
}


TEST_P(SomewhatSchemeCircuitsTest, MultiplicationOneZero) {
    SomewhatScheme scheme(7, GetParam());
    auto [sk, pk] = scheme.key_gen();

    auto m1 = NTL::GF2(1);
    auto m2 = NTL::GF2(0);

    auto c1 = scheme.encrypt(pk, m1);
    auto c2 = scheme.encrypt(pk, m2);

    auto m1_out = mul_circuit(m1, m2);
    auto c1_out = mul_circuit(c1, c2);
    auto d1_out = scheme.decrypt(sk, c1_out);

    EXPECT_EQ(m1_out, d1_out);
}

TEST_P(SomewhatSchemeCircuitsTest, MultiplicationOnes) {
    SomewhatScheme scheme(7, GetParam());
    auto [sk, pk] = scheme.key_gen();

    auto m1 = NTL::GF2(1);
    auto m2 = NTL::GF2(1);

    auto c1 = scheme.encrypt(pk, m1);
    auto c2 = scheme.encrypt(pk, m2);

    auto m1_out = mul_circuit(m1, m2);
    auto c1_out = mul_circuit(c1, c2);
    auto d1_out = scheme.decrypt(sk, c1_out);

    EXPECT_EQ(m1_out, d1_out);
}

TEST_P(SomewhatSchemeCircuitsTest, Circuit1) {
    SomewhatScheme scheme(7, GetParam());
    auto [sk, pk] = scheme.key_gen();

    auto m1 = NTL::GF2(1);
    auto m2 = NTL::GF2(1);
    auto m3 = NTL::GF2(0);

    auto c1 = scheme.encrypt(pk, m1);
    auto c2 = scheme.encrypt(pk, m2);
    auto c3 = scheme.encrypt(pk, m3);

    auto m1_out = circuit1(m1, m2, m3);
    auto c1_out = circuit1(c1, c2, c3);
    auto d1_out = scheme.decrypt(sk, c1_out);

    EXPECT_EQ(m1_out, d1_out);
}

TEST_P(SomewhatSchemeCircuitsTest, Circuit2) {
    SomewhatScheme scheme(8, GetParam());
    auto [sk, pk] = scheme.key_gen();

    auto m1 = NTL::GF2(1);
    auto m2 = NTL::GF2(1);
    auto m3 = NTL::GF2(0);

    auto c1 = scheme.encrypt(pk, m1);
    auto c2 = scheme.encrypt(pk, m2);
    auto c3 = scheme.encrypt(pk, m3);

    auto m1_out = circuit2(m1, m2, m3);
    auto c1_out = circuit2(c1, c2, c3);
    auto d1_out = scheme.decrypt(sk, c1_out);

    EXPECT_EQ(m1_out, d1_out);
}

TEST_P(SomewhatSchemeCircuitsTest, Circuit3) {
    SomewhatScheme scheme(8, GetParam());
    auto [sk, pk] = scheme.key_gen();

    auto m1 = NTL::GF2(1);
    auto m2 = NTL::GF2(0);
    auto m3 = NTL::GF2(1);
    auto m4 = NTL::GF2(1);
    auto m5 = NTL::GF2(1);
    auto m6 = NTL::GF2(0);

    auto c1 = scheme.encrypt(pk, m1);
    auto c2 = scheme.encrypt(pk, m2);
    auto c3 = scheme.encrypt(pk, m3);
    auto c4 = scheme.encrypt(pk, m4);
    auto c5 = scheme.encrypt(pk, m5);
    auto c6 = scheme.encrypt(pk, m6);

    auto m_out = circuit3(m1, m2, m3, m4, m5, m6);
    auto c_out = circuit3(c1, c2, c3, c4, c5, c6);
    auto d_out = scheme.decrypt(sk, c_out);

    EXPECT_EQ(m_out, d_out);
}

class KeysAndCiphertextsRanges : public ::testing::TestWithParam<std::pair<int, long>> {
};

INSTANTIATE_TEST_SUITE_P(RandomSeeds, KeysAndCiphertextsRanges, ::testing::Values(
        std::pair{6, 0},
        std::pair{6, 1688384936},
        std::pair{8, 0},
        std::pair{8, 1},
        std::pair{8, 1688384934},
        std::pair{9, 123}
));

TEST_P(KeysAndCiphertextsRanges, SampleSecretKey) {
    auto param = GetParam();
    SomewhatScheme scheme(param.first, param.second);
    mpz_class p = scheme.sample_secret_key();
    mpz_class right = pow_of_two(scheme.eta);
    mpz_class left = pow_of_two(scheme.eta - 1);

    EXPECT_TRUE(p % 2 == 1);
    EXPECT_TRUE(p < right);
    EXPECT_TRUE(p >= left);
}

TEST_P(KeysAndCiphertextsRanges, SamplePublicKey) {
    auto param = GetParam();
    SomewhatScheme scheme(param.first, param.second);
    mpz_class p = scheme.sample_secret_key();
    auto x = scheme.sample_public_key(p);
    mpz_class right = pow_of_two(scheme.gamma);
    mpz_class left = -pow_of_two(scheme.rho);

    EXPECT_EQ(x.size(), scheme.tau + 1);
    for (const auto &i: x) {
        EXPECT_TRUE(i < right);
        EXPECT_TRUE(i > left);
    }
    int max_idx = x.begin() - std::max_element(x.begin(), x.end());
    EXPECT_EQ(max_idx, 0);
    EXPECT_TRUE(x[0] % 2 == 1);
    EXPECT_TRUE(rem(x[0], p) % 2 == 0);
}

TEST_P(KeysAndCiphertextsRanges, Encrypt) {
    auto param = GetParam();
    SomewhatScheme scheme(param.first, param.second);
    auto [secret_key, public_key] = scheme.key_gen();
    auto m0 = NTL::GF2{0};
    auto m1 = NTL::GF2{1};
    auto c0 = scheme.encrypt(public_key, m0);
    auto c1 = scheme.encrypt(public_key, m1);
    mpz_class range = public_key.pk[0];

    EXPECT_TRUE(abs(c0) < range);
    EXPECT_TRUE(abs(c1) < range);
}

class CiphertextModularReduction : public ::testing::TestWithParam<std::pair<int, long>> {
};

INSTANTIATE_TEST_SUITE_P(RandomSeeds, CiphertextModularReduction, ::testing::Values(
        std::pair{6, 0},
        std::pair{6, 1},
        std::pair{6, 2},
        std::pair{6, 3},
        std::pair{7, 0},
        std::pair{6, 1688384936},
        std::pair{8, 0},
        std::pair{8, 1},
        std::pair{8, 1688384934},
        std::pair{9, 123}
));

TEST_P(CiphertextModularReduction, CiphertextsHoldReductionElements) {
    auto param = GetParam();
    SomewhatScheme scheme(param.first, param.second);
    auto [p, public_key] = scheme.key_gen();
    auto m0 = NTL::GF2{0};
    auto m1 = NTL::GF2{1};

    auto c0 = scheme.encrypt(public_key, m0);
    auto c1 = scheme.encrypt(public_key, m1);
    auto c2 = c1;

    auto threshold = pow_of_two(scheme.gamma);

    EXPECT_TRUE(c0.mod_red == public_key.mod_red);
    EXPECT_TRUE(c1.mod_red == public_key.mod_red);
    EXPECT_TRUE(c2.mod_red == public_key.mod_red);
}

TEST_P(CiphertextModularReduction, CiphertextReducesAfterExceedingThreshold) {
    auto param = GetParam();
    SomewhatScheme scheme(param.first, param.second);
    auto [p, public_key] = scheme.key_gen();
    auto m0 = NTL::GF2{0};
    auto m1 = NTL::GF2{1};

    auto c0 = scheme.encrypt(public_key, m0);
    auto c1 = scheme.encrypt(public_key, m1);

    auto c2 = c0 * c1;

    auto d_c0 = scheme.decrypt(p, c0);
    auto d_c1 = scheme.decrypt(p, c1);
    auto d_c2 = scheme.decrypt(p, c2);

    auto threshold = pow_of_two(scheme.gamma);
    EXPECT_EQ(d_c0, m0);
    EXPECT_EQ(d_c1, m1);
    EXPECT_EQ(d_c2, m1 * m0);
    EXPECT_TRUE(abs(c2) < threshold);
}

