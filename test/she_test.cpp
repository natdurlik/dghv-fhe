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

TEST_P(SomewhatSchemeCircuitsTest, AdditionZeroOne) {
    SomewhatScheme scheme(7, GetParam());

    mpz_class sk;
    std::vector<mpz_class> pk;
    tie(sk, pk) = scheme.key_gen();

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

    mpz_class sk;
    std::vector<mpz_class> pk;
    tie(sk, pk) = scheme.key_gen();

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

    mpz_class sk;
    std::vector<mpz_class> pk;
    tie(sk, pk) = scheme.key_gen();

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

    mpz_class sk;
    std::vector<mpz_class> pk;
    tie(sk, pk) = scheme.key_gen();

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

    mpz_class sk;
    std::vector<mpz_class> pk;
    tie(sk, pk) = scheme.key_gen();

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

    mpz_class sk;
    std::vector<mpz_class> pk;
    tie(sk, pk) = scheme.key_gen();

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

    mpz_class sk;
    std::vector<mpz_class> pk;
    tie(sk, pk) = scheme.key_gen();

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

INSTANTIATE_TEST_SUITE_P(RandomSeeds, SomewhatSchemeCircuitsTest, ::testing::Values(0, 1, 2, 3, 4, 5));
