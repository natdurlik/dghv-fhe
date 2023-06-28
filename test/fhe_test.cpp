#include <gtest/gtest.h>
#include "FullyScheme.h"
#include "utils.h"

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

TEST(HammingWeightCiphertext, SmallNumbers) {
//    long seed = time(nullptr);
    long seed = 1687810596;
    // 1687810596 fast seed
    std::cerr << "with seed = " << seed << std::endl;
    FullyScheme fhe = FullyScheme(11, seed);
    SecretKey secret_key;
    PublicKey public_key;
    std::tie(secret_key, public_key) = fhe.key_gen();

    // 1
    std::vector<mpz_class> a = {fhe.encrypt(public_key.pk, NTL::GF2{1})};
    auto out = fhe.hamming_weight(a);
    EXPECT_EQ(fhe.decrypt(secret_key.p, out[0]), NTL::GF2{1});
    for (int i = 1; i < out.size(); i++) {
        EXPECT_EQ(fhe.decrypt(secret_key.p, out[i]), NTL::GF2{0});
    }

    // 0
    a = {fhe.encrypt(public_key.pk, NTL::GF2{0}),
         fhe.encrypt(public_key.pk, NTL::GF2{0}), fhe.encrypt(public_key.pk, NTL::GF2{0})};
    out = fhe.hamming_weight(a);
    for (const auto &i: out) {
        EXPECT_EQ(fhe.decrypt(secret_key.p, i), NTL::GF2{0});
    }

    // 1
    a = {fhe.encrypt(public_key.pk, NTL::GF2{1}), fhe.encrypt(public_key.pk, NTL::GF2{0}),
         fhe.encrypt(public_key.pk, NTL::GF2{0}), fhe.encrypt(public_key.pk, NTL::GF2{0}),
         fhe.encrypt(public_key.pk, NTL::GF2{0}), fhe.encrypt(public_key.pk, NTL::GF2{0})};
    out = fhe.hamming_weight(a);
    EXPECT_EQ(fhe.decrypt(secret_key.p, out[0]), NTL::GF2{1});
    for (int i = 1; i < out.size(); i++) {
        EXPECT_EQ(fhe.decrypt(secret_key.p, out[i]), NTL::GF2{0});
    }

    // 3
    a = {fhe.encrypt(public_key.pk, NTL::GF2{1}), fhe.encrypt(public_key.pk, NTL::GF2{1}),
         fhe.encrypt(public_key.pk, NTL::GF2{0}), fhe.encrypt(public_key.pk, NTL::GF2{1})};
    out = fhe.hamming_weight(a);
    EXPECT_EQ(fhe.decrypt(secret_key.p, out[0]), NTL::GF2{1});
    EXPECT_EQ(fhe.decrypt(secret_key.p, out[1]), NTL::GF2{1});
    for (int i = 2; i < out.size(); i++) {
        EXPECT_EQ(fhe.decrypt(secret_key.p, out[i]), NTL::GF2{0});
    }
}

TEST(SetthetaBitsToOne, SimpleCheck) {
    FullyScheme fhe(8, 0);
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
    FullyScheme fhe(8, 0);

    int Theta = 100;
    std::vector<NTL::GF2> s(Theta, NTL::GF2{0});
    s[1] = s[10] = s[32] = s[45] = s[98] = NTL::GF2{1};

    unsigned long kappa = 100;
    mpz_class sum_to = pow_of_two(kappa);
    sum_to /= 23;

    mpz_class range = pow_of_two(kappa + 1);

    std::vector<mpz_class> u(Theta);
    fhe.fill_with_random_integers_with_condition(u, s, range, sum_to);

    mpz_class sum = 0;
    for (int i = 0; i < u.size(); i++) {
        if (IsOne(s[i])) {
            sum += u[i];
        }
    }

    sum %= range;
    EXPECT_EQ(sum, sum_to);
}

TEST(KeyGen, PublicKeyHoldsProperEncryptionOfSecretKey) {
    FullyScheme fhe(8, 0);
    SecretKey secret_key;
    PublicKey public_key;
    std::tie(secret_key, public_key) = fhe.key_gen();

    for (int i = 0; i < secret_key.s.size(); i++) {
        NTL::GF2 m = fhe.decrypt(secret_key.p, public_key.e_sk[i]);
        EXPECT_EQ(m, secret_key.s[i]);
    }
}

TEST(KeyGen, PublicKeyYsInCorrectRange) {
    FullyScheme fhe = FullyScheme(8, 0);
    SecretKey secret_key;
    PublicKey public_key;
    std::tie(secret_key, public_key) = fhe.key_gen();

    EXPECT_EQ(public_key.y.size(), fhe.Theta);
    for (const auto &y: public_key.y) {
        EXPECT_TRUE(y < 2);
        EXPECT_TRUE(y >= 0);
        EXPECT_TRUE(y.get_prec() >= fhe.kappa);
    }
}


TEST(KeyGen, PublicKeySumsToOneOverP) {
    long seed = time(nullptr);
    std::clog << seed << std::endl;
    FullyScheme fhe(8, seed); // fixme
    auto [secret_key, public_key] = fhe.key_gen();

    mpf_class sum(0, fhe.kappa + 2);
    mpf_class x(1, fhe.kappa + 2);
    x /= secret_key.p;
    for (int i = 0; i < public_key.y.size(); i++) {
        if (IsOne(secret_key.s[i])) {
            sum += public_key.y[i];
            sum = mod2f(sum);
        }
    }

    mpf_class err(1, fhe.kappa);
    err /= pow_of_two(fhe.kappa);

    EXPECT_TRUE(abs(x - sum) < err);
}

TEST(PostProcess, RationalBitsSumWithinQuarterOfAnInteger) {
    long seed = time(nullptr);
    std::clog << seed << std::endl;
    FullyScheme fhe(8, seed);
    auto [secret_key, public_key] = fhe.key_gen();
    auto c0 = fhe.encrypt(public_key.pk, NTL::GF2{0});

    auto [c_star, z] = fhe.post_process(c0, public_key.y);
    mpf_class sum(0, fhe.n + 1);

    for (int i = 0; i < z.size(); i++) {
        if (IsOne(secret_key.s[i])) {
            mpf_class curr = bits_to_mpf(z[i], fhe.n + 1);
            sum += curr;
            sum = mod2f(sum);
        }
    }

    auto bits = mpf_to_bits(sum, 5);
    EXPECT_EQ(bits[1], bits[2]);
}

class PostProcessCases : public ::testing::TestWithParam<std::pair<int, long>> {
};

TEST_P(PostProcessCases, RationalBitsSumWithinQuarterOfAnInteger) {
    const auto &param = GetParam();
    FullyScheme fhe(param.first, param.second);
    auto [secret_key, public_key] = fhe.key_gen();
    auto c0 = fhe.encrypt(public_key.pk, NTL::GF2{0});

    auto [c_star, z] = fhe.post_process(c0, public_key.y);
    mpf_class sum(0, fhe.n + 1);

    for (int i = 0; i < z.size(); i++) {
        if (IsOne(secret_key.s[i])) {
            mpf_class curr = bits_to_mpf(z[i], fhe.n + 1);
            sum += curr;
            sum = mod2f(sum);
        }
    }

    auto bits = mpf_to_bits(sum, 4);

    EXPECT_EQ(bits[1], bits[2]);
}


INSTANTIATE_TEST_SUITE_P(SchemeParameters, PostProcessCases, ::testing::Values(
        std::pair{8, 0},
        std::pair{8, 1},
        std::pair{8, 3},
        std::pair{8, 4},
        std::pair{9, 0},
        std::pair{9, 1},
        std::pair{9, 2},
        std::pair{10, 1687810596}
));

TEST_P(PostProcessCases, RationalBitsSumRoundsToCOverP) {
    const auto &param = GetParam();
    FullyScheme fhe(param.first, param.second);
    auto [secret_key, public_key] = fhe.key_gen();
    auto c = fhe.encrypt(public_key.pk, NTL::GF2{1});

    auto [c_star, z] = fhe.post_process(c, public_key.y);
    mpf_class sum(0, fhe.n + 1);

    for (int i = 0; i < z.size(); i++) {
        if (IsOne(secret_key.s[i])) {
            mpf_class curr = bits_to_mpf(z[i], fhe.n + 1);
            sum += curr;
            sum = mod2f(sum);
        }
    }
    mpz_class rounded_sum = round_to_closest(sum) % 2;

    mpf_class c_div_p(c, fhe.kappa + 2);
    c_div_p /= secret_key.p;
    c_div_p = mod2f(c_div_p);
    mpz_class rounded_cp = round_to_closest(c_div_p) % 2;

    EXPECT_EQ(rounded_cp, rounded_sum);
}

TEST(ShiftAndMod2, SimpleChecks) {
    std::vector<std::vector<NTL::GF2>> W{
            {NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{0}},
            {NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{0}},
            {NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{0}},
            {NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{0}},
            {NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{0}},
            {NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{0}},
            {NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{0}},
    };

    FullyScheme fhe(8, 0);
    auto out = fhe.shift_and_mod2(W, 7);
    std::vector<std::vector<NTL::GF2>> w{
            {NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}},
            {NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}},
            {NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}},
            {NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}},
            {NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}},
            {NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{0}},
            {NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{0}},
    };
    EXPECT_EQ(out.size(), W.size());

    for (int i = 0; i < w.size(); i++) {
        EXPECT_EQ(out[i].size(), w[i].size());
        for (int j = 0; j < w[i].size(); j++) {
            EXPECT_EQ(out[i][j], w[i][j]);
        }
    }
}

class GenerateFewerNumbers : public ::testing::TestWithParam<std::pair<int, long>> {
};

TEST_P(GenerateFewerNumbers, OutputHasTheSameSum) {
    const auto &param = GetParam();
    long seed = param.first;
    FullyScheme fhe(8, seed);
    auto [secret_key, public_key] = fhe.key_gen();
    auto c = fhe.encrypt(public_key.pk, NTL::GF2{param.second});

    auto [c_star, z] = fhe.post_process(c, public_key.y);

    auto a = z;
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a[i].size(); j++) {
            a[i][j] *= secret_key.s[i];
        }
    }

    auto w = fhe.generate_fewer_numbers(a);

    mpf_class w_sum(0, fhe.n + 1);
    mpf_class a_sum(0, fhe.n + 1);
    mpf_class curr(0, fhe.n + 1);

    for (int i = 0; i < a.size(); i++) {
        curr = bits_to_mpf(a[i], fhe.n + 1);
        a_sum += curr;
        a_sum = mod2f(a_sum);
    }

    for (int i = 0; i < w.size(); i++) {
        curr = bits_to_mpf(w[i], fhe.n + 1);
        w_sum += curr;
        w_sum = mod2f(w_sum);
    }

    mpf_class err(0.000000001, 100);

    EXPECT_TRUE(abs(w_sum - a_sum) < err);
}

INSTANTIATE_TEST_SUITE_P(SchemeParameters, GenerateFewerNumbers, ::testing::Values(
        std::pair{0, 0},
        std::pair{0, 1},
        std::pair{1, 1},
        std::pair{1687900131, 0},
        std::pair{1687899741, 0},
        std::pair{1687899700, 1}
));