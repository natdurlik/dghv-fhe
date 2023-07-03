#include <gtest/gtest.h>
#include "FullyScheme.h"
#include "utils.h"

TEST(HammingWeightBinary, AllZeros) {
    FullyScheme fhe(6, 35, 35, 0);
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
    FullyScheme fhe(6, 35, 35, 0);
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
    FullyScheme fhe(8, 35, 35, 0);
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
//    std::cerr << "with seed = " << seed << std::endl;
    FullyScheme fhe = FullyScheme(6, 35, 35, seed);
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
    FullyScheme fhe(6, 35, 35, 0);
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
    FullyScheme fhe(6, 35, 35, 0);

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

class KeyGen : public ::testing::TestWithParam<std::pair<int, long>> {
};

INSTANTIATE_TEST_SUITE_P(SchemeParameters, KeyGen, ::testing::Values(
        std::pair{6, 0},
        std::pair{6, 1},
        std::pair{6, 3},
        std::pair{6, 4},
        std::pair{7, 0},
        std::pair{7, 1},
        std::pair{8, 2}
));

TEST_P(KeyGen, PublicKeyHoldsProperEncryptionOfSecretKey) {
    auto param = GetParam();
    FullyScheme fhe(param.first, 35, 35, param.second);
    SecretKey secret_key;
    PublicKey public_key;
    std::tie(secret_key, public_key) = fhe.key_gen();

    for (int i = 0; i < secret_key.s.size(); i++) {
        NTL::GF2 m = fhe.decrypt(secret_key.p, public_key.e_sk[i]);
        EXPECT_EQ(m, secret_key.s[i]);
    }
}

TEST_P(KeyGen, PublicKeyYsInCorrectRange) {
    auto param = GetParam();
    FullyScheme fhe = FullyScheme(param.first, 35, 35, param.second);
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

TEST_P(KeyGen, PublicKeySumsToOneOverP) {
    auto param = GetParam();
    long seed = param.second;
    FullyScheme fhe(param.first, 35, 35, seed);
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
    FullyScheme fhe(6, 35, 35, seed);
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
    FullyScheme fhe(param.first, 35, 35, param.second);
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
        std::pair{6, 0},
        std::pair{6, 1},
        std::pair{6, 3},
        std::pair{6, 4},
        std::pair{7, 0},
        std::pair{7, 1},
        std::pair{8, 2}
));

TEST_P(PostProcessCases, RationalBitsSumRoundsToCOverP) {
    const auto &param = GetParam();
    FullyScheme fhe(param.first, 35, 35, param.second);
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

    FullyScheme fhe(8, 35, 35, 0);
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

INSTANTIATE_TEST_SUITE_P(SchemeParameters, GenerateFewerNumbers, ::testing::Values(
        std::pair{0, 0},
        std::pair{0, 1},
        std::pair{1, 1},
        std::pair{1687900131, 0},
        std::pair{1687899741, 0},
        std::pair{1687899700, 1}
));

TEST_P(GenerateFewerNumbers, OutputHasTheSameSum) {
    auto param = GetParam();
    long seed = param.first;
    FullyScheme fhe(6, 35, 35, seed);
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

TEST(CarrySaveAdderMod2, SimpleChecks) {
    FullyScheme fhe(6, 35, 35, 0);

    std::vector<NTL::GF2> a{NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{1}};
    std::vector<NTL::GF2> b{NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{0}};
    std::vector<NTL::GF2> c{NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{1}};

    auto [out_sum, out_carry] = fhe.carry_save_adder_mod2(a, b, c);
    std::vector<NTL::GF2> sum{NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}};
    std::vector<NTL::GF2> carry{NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{0}};

    EXPECT_EQ(out_sum.size(), sum.size());
    for (int i = 0; i < sum.size(); i++) {
        EXPECT_EQ(out_sum[i], sum[i]);
    }

    EXPECT_EQ(out_carry.size(), carry.size());
    for (int i = 0; i < carry.size(); i++) {
        EXPECT_EQ(out_carry[i], carry[i]);
    }
}

TEST(KForTwo, ProducesValidSumMod2) {
    FullyScheme fhe(6, 35, 35, 0);
    int prec = 100;
    std::vector<mpf_class> rationals{mpf_class(0.125, prec), mpf_class(1.875, prec), mpf_class(1.5, prec),
                                     mpf_class(0.25, prec), mpf_class(0.6875, prec), mpf_class(1.4375, prec)};
    std::vector<std::vector<NTL::GF2>> w;
    for (const auto &rational: rationals) {
        w.push_back(mpf_to_bits(rational, 5));
    }

    auto [s1, s2] = fhe.k_for_two(w);
    mpf_class out_sum = bits_to_mpf(s1, prec) + bits_to_mpf(s2, prec);
    mpf_class sum(0, prec);
    for (const auto &rational: rationals) {
        sum += rational;
        sum = mod2f(sum);
    }

    mpf_class err(0.000001, prec);
    EXPECT_TRUE(abs(out_sum - sum) < err);
}

TEST(OrT, SimpleCheck) {
    FullyScheme fhe(6, 35, 35, 0);
    NTL::GF2 a{0};
    NTL::GF2 b{0};
    EXPECT_EQ(fhe.or_t(a, b), NTL::GF2{0});

    a = NTL::GF2{1};
    b = NTL::GF2{1};
    EXPECT_EQ(fhe.or_t(a, b), NTL::GF2{1});

    a = NTL::GF2{1};
    b = NTL::GF2{0};
    EXPECT_EQ(fhe.or_t(a, b), NTL::GF2{1});

    a = NTL::GF2{0};
    b = NTL::GF2{1};
    EXPECT_EQ(fhe.or_t(a, b), NTL::GF2{1});
}

class SquashedDecrypt : public ::testing::TestWithParam<std::tuple<int, long, int>> {
};

INSTANTIATE_TEST_SUITE_P(SimpleChecks, SquashedDecrypt, ::testing::Values(
        std::tuple{6, 0, 0},
        std::tuple{6, 0, 1},
        std::tuple{6, 2, 1},
        std::tuple{6, 2, 0},
        std::tuple{6, 3, 0},
        std::tuple{6, 3, 1},
        std::tuple{8, 1687899700, 1},
        std::tuple{8, 1687899700, 0},
        std::tuple{6, 1687899613, 0},
        std::tuple{6, 1687899613, 1}
));

TEST_P(SquashedDecrypt, SimpleChecks) {
    auto param = GetParam();
    FullyScheme fhe(std::get<0>(param), 35, 35, std::get<1>(param));
    auto [secret_key, public_key] = fhe.key_gen();

    auto message = NTL::GF2{std::get<2>(param)};
    auto c = fhe.encrypt(public_key.pk, message);
    auto [c_star, z] = fhe.post_process(c, public_key.y);
    auto decrypted = fhe.squashed_decrypt(c_star, secret_key.s, z);

    EXPECT_EQ(decrypted, message);
}

class Recrypt : public ::testing::TestWithParam<std::tuple<int, long, int>> {
};

INSTANTIATE_TEST_SUITE_P(SimpleChecks, Recrypt, ::testing::Values(
        std::tuple{6, 0, 0},
        std::tuple{6, 0, 1},
        std::tuple{6, 1, 1},
        std::tuple{6, 1, 0},
        std::tuple{6, 2, 0},
        std::tuple{6, 2, 1},
        std::tuple{6, 1687899700, 1},
        std::tuple{6, 1687899700, 0},
        std::tuple{6, 1687899613, 0},
        std::tuple{6, 1687899613, 1}
));

TEST_P(Recrypt, SimpleChecks) {
    auto param = GetParam();
    FullyScheme fhe(std::get<0>(param), 35, 35, std::get<1>(param));
    auto [secret_key, public_key] = fhe.key_gen();
    auto message = NTL::GF2{std::get<2>(param)};

    auto c = fhe.encrypt(public_key.pk, message);
    auto recrypted = fhe.recrypt(c, public_key);
    auto decrypted = fhe.decrypt(secret_key.p, recrypted);

    EXPECT_EQ(decrypted, message);
}

class Circuits : public ::testing::TestWithParam<std::pair<int, long>> {
};

INSTANTIATE_TEST_SUITE_P(SimpleChecks, Circuits, ::testing::Values(
        std::pair{6, 0},
        std::pair{6, 1},
        std::pair{6, 2},
        std::pair{6, 3},
        std::pair{6, 4},
        std::pair{6, 5},
        std::pair{7, 1687900131}
));

template<typename T>
T mul_circuit(T m1, T m2) {
    return m1 * m2;
}

void
mul_aux(FullyScheme &fhe, const PublicKey &public_key, const SecretKey &secret_key, NTL::GF2 m1, NTL::GF2 m2) {
    auto m_out = mul_circuit(m1, m2);

    auto c1 = fhe.encrypt(public_key.pk, m1);
    auto c2 = fhe.encrypt(public_key.pk, m2);
    auto c_out = mul_circuit(c1, c2);

    auto [c_star, z] = fhe.post_process(c_out, public_key.y);
    auto d_out = fhe.decrypt(secret_key.p, c_out);
    auto s_out = fhe.squashed_decrypt(c_star, secret_key.s, z);

    auto recrypted = fhe.recrypt(c_out, public_key);
    auto [c_star_r, z_r] = fhe.post_process(recrypted, public_key.y);
    auto r_out = fhe.squashed_decrypt(c_star_r, secret_key.s, z_r);
//    auto r_out = fhe.decrypt(secret_key.p, recrypted);

    EXPECT_EQ(d_out, m_out) << "normal decrypt";
    EXPECT_EQ(s_out, m_out) << "squash decrypt";
    EXPECT_EQ(r_out, m_out) << "recrypt decrypt";
}

template<typename T>
T add_circuit(T m1, T m2) {
    return m1 + m2;
}

void
add_aux(FullyScheme &fhe, const PublicKey &public_key, const SecretKey &secret_key, NTL::GF2 m1, NTL::GF2 m2) {
    auto m_out = add_circuit(m1, m2);

    auto c1 = fhe.encrypt(public_key.pk, m1);
    auto c2 = fhe.encrypt(public_key.pk, m2);
    auto c_out = add_circuit(c1, c2);

    auto [c_star, z] = fhe.post_process(c_out, public_key.y);
    auto d_out = fhe.decrypt(secret_key.p, c_out);
    auto s_out = fhe.squashed_decrypt(c_star, secret_key.s, z);

    auto recrypted = fhe.recrypt(c_out, public_key);
    auto [c_star_r, z_r] = fhe.post_process(recrypted, public_key.y);
    auto r_out = fhe.squashed_decrypt(c_star_r, secret_key.s, z_r);
//    auto r_out = fhe.decrypt(secret_key.p, recrypted);

    EXPECT_EQ(d_out, m_out) << "normal decrypt";
    EXPECT_EQ(s_out, m_out) << "squash decrypt";
    EXPECT_EQ(r_out, m_out) << "recrypt decrypt";
}

TEST_P(Circuits, Mul) {
    auto param = GetParam();
    FullyScheme fhe(param.first, 35, 35, param.second);
//    fhe.kappa = fhe.gamma * fhe.eta;
    auto [secret_key, public_key] = fhe.key_gen();
    mul_aux(fhe, public_key, secret_key, NTL::GF2{1}, NTL::GF2{1});
    mul_aux(fhe, public_key, secret_key, NTL::GF2{1}, NTL::GF2{0});
    mul_aux(fhe, public_key, secret_key, NTL::GF2{0}, NTL::GF2{1});
    mul_aux(fhe, public_key, secret_key, NTL::GF2{0}, NTL::GF2{0});
}

TEST_P(Circuits, Add) {
    auto param = GetParam();
    FullyScheme fhe(param.first, 35, 35, param.second);
    auto [secret_key, public_key] = fhe.key_gen();
    add_aux(fhe, public_key, secret_key, NTL::GF2{1}, NTL::GF2{1});
    add_aux(fhe, public_key, secret_key, NTL::GF2{1}, NTL::GF2{0});
    add_aux(fhe, public_key, secret_key, NTL::GF2{0}, NTL::GF2{1});
    add_aux(fhe, public_key, secret_key, NTL::GF2{0}, NTL::GF2{0});
}

TEST_P(Circuits, ArbitraryLongMulCircuit) {
    auto param = GetParam();
    FullyScheme fhe(param.first, 35, 35, param.second);
    auto [secret_key, public_key] = fhe.key_gen();
    auto m1 = NTL::GF2{1};
    auto m2 = NTL::GF2{1};
    auto c1 = fhe.encrypt(public_key.pk, m1);
    auto c2 = fhe.encrypt(public_key.pk, m2);
    auto c1_recrypted = fhe.recrypt(c1, public_key);
    auto c2_recrypted = fhe.recrypt(c2, public_key);

    for (int i = 0; i < 20; i++) {
        auto m3 = m1 * m2;
        auto c3 = c1_recrypted * c2_recrypted;

        auto c3_d = fhe.decrypt(secret_key.p, c3);
        auto [c_star, z] = fhe.post_process(c3, public_key.y);
        auto c3_s_d = fhe.squashed_decrypt(c_star, secret_key.s, z);
        ASSERT_EQ(c3_d, m3) << "normal decrypt";
        ASSERT_EQ(c3_s_d, m3) << "squashed decrypt";
        c1_recrypted = fhe.recrypt(c3, public_key);
        c2_recrypted = fhe.recrypt(c2, public_key);
    }
}

TEST_P(Recrypt, NoiseIsSmallerAfterRecrypt) {
    auto param = GetParam();
    FullyScheme fhe(std::get<0>(param), 35, 35, std::get<1>(param));
    auto [secret_key, public_key] = fhe.key_gen();
    auto m = NTL::GF2{std::get<2>(param)};
    auto c = fhe.encrypt(public_key.pk, m);
    c = fhe.recrypt(c, public_key);

    // some operation
    c *= c;
    mpz_class noise = rem(c, secret_key.p);

    c = fhe.recrypt(c, public_key);
    mpz_class noise_recrypted = rem(c, secret_key.p);

    EXPECT_TRUE(abs(noise) > abs(noise_recrypted));
}
