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
    FullyScheme fhe(9, 0);
    SecretKey secret_key;
    PublicKey public_key;
    std::tie(secret_key, public_key) = fhe.key_gen();

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
