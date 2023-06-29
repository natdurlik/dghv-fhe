#include <gtest/gtest.h>
#include "utils.h"

class QuotTest : public ::testing::TestWithParam<std::tuple<std::string, std::string, int>> {
};

TEST_P(QuotTest, BasicCases) {
    const auto &param = GetParam();
    mpz_class a(std::get<0>(param));
    mpz_class b(std::get<1>(param));
    mpz_class res = quot(a, b);

    EXPECT_EQ(res, std::get<2>(param));
}

INSTANTIATE_TEST_SUITE_P(SmallInts, QuotTest, ::testing::Values(
        std::tuple{"7", "2", 4},
        std::tuple{"3", "6", 1},
        std::tuple{"10", "1", 10},
        std::tuple{"1", "10", 0},
        std::tuple{"1", "2", 1},
        std::tuple{"-1", "2", 0},
        std::tuple{"1", "-2", 0},
        std::tuple{"-1", "-2", 1},
        std::tuple{"-10", "2", -5},
        std::tuple{"10", "-2", -5},
        std::tuple{"-20", "3", -7},
        std::tuple{"20", "3", 7},
        std::tuple{"-20", "-3", 7},
        std::tuple{"11", "2", 6},
        std::tuple{"-11", "2", -5}
));

class RemTest : public ::testing::TestWithParam<std::tuple<std::string, std::string, int>> {
};

TEST_P(RemTest, BasicCases) {
    const auto &param = GetParam();
    mpz_class a(std::get<0>(param));
    mpz_class b(std::get<1>(param));
    mpz_class res = rem(a, b);

    EXPECT_EQ(res, std::get<2>(param));
}

INSTANTIATE_TEST_SUITE_P(SmallInts, RemTest, ::testing::Values(
        std::tuple{"7", "2", -1},
        std::tuple{"3", "6", -3},
        std::tuple{"10", "1", 0},
        std::tuple{"1", "10", 1},
        std::tuple{"1", "2", -1},
        std::tuple{"-1", "2", -1},
        std::tuple{"1", "-2", 1},
        std::tuple{"-1", "-2", 1},
        std::tuple{"-10", "2", 0},
        std::tuple{"-10", "-2", 0},
        std::tuple{"10", "-2", 0},
        std::tuple{"-20", "3", 1},
        std::tuple{"20", "3", -1},
        std::tuple{"-20", "-3", 1},
        std::tuple{"20", "-3", -1},
        std::tuple{"11", "2", -1},
        std::tuple{"-11", "2", -1}
));

TEST(IsEven, SimpleChecks) {
    EXPECT_EQ(is_even(0), true);
    EXPECT_EQ(is_even(2), true);
    EXPECT_EQ(is_even(-10), true);
    EXPECT_EQ(is_even(1), false);
    EXPECT_EQ(is_even(-1), false);
    EXPECT_EQ(is_even(-33), false);
}

TEST(PowOfTwo, SimpleChecks) {
    EXPECT_EQ(pow_of_two(2), 4);
    EXPECT_EQ(pow_of_two(3), 8);
    EXPECT_EQ(pow_of_two(10), 1024);
}

TEST(Mod2f, SimpleChecks) {
    int prec = 100;
    mpf_class err(0.00001, prec); // fixme?

    mpf_class x(123, prec);
    mpf_class res(1, prec);
    EXPECT_TRUE(abs(mod2f(x) - res) < err);

    x = 12.33456;
    res = 0.33456;
    EXPECT_TRUE(abs(mod2f(x) - res) < err);

    x = 0.1;
    res = 0.1;
    EXPECT_TRUE(abs(mod2f(x) - res) < err);

    x = 2.123;
    res = 0.123;
    EXPECT_TRUE(abs(mod2f(x) - res) < err);
}

TEST(Mod2f, NegativeValues) {
    int prec = 100;
    mpf_class err(0.00001, prec); // fixme?

    mpf_class x(-1.123, prec);
    mpf_class res(2, prec);
    res += x;
    EXPECT_TRUE(abs(mod2f(x) - res) < err);

    x = -0.123;
    res = 2;
    res += x;
    EXPECT_TRUE(abs(mod2f(x) - res) < err);

    x = -120.123;
    res = 1.877;
    EXPECT_TRUE(abs(mod2f(x) - res) < err);

    x = -121111.5;
    res = 0.5;
    EXPECT_TRUE(abs(mod2f(x) - res) < err);

}


void to_bits_check_aux(const std::vector<NTL::GF2> &bits, const std::vector<NTL::GF2> &ans) {
    for (int i = 0; i < ans.size(); i++) {
        EXPECT_EQ(bits[i], ans[i]);
    }
    for (int i = ans.size(); i < bits.size(); i++) {
        EXPECT_EQ(bits[i], NTL::GF2{0});
    }
}

TEST(MpfToBits, SimpleChecks) {
    int n = 8;

    mpf_class f(0.5, n);
    auto bits = mpf_to_bits(f, n);

    EXPECT_EQ(bits.size(), n);
    std::vector<NTL::GF2> ans{NTL::GF2{0}, NTL::GF2{1}};
    to_bits_check_aux(bits, ans);

    f = 0.125;
    bits = mpf_to_bits(f, n);
    ans = {NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{1}};
    EXPECT_EQ(bits.size(), n);
    to_bits_check_aux(bits, ans);

    f = 1.03125;
    bits = mpf_to_bits(f, n);
    ans = {NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{1}};
    EXPECT_EQ(bits.size(), n);
    to_bits_check_aux(bits, ans);
}

TEST(MpfToBits, ThrowsWhenGreaterOrEqualTwo) {
    int n = 8;
    mpf_class f(2.125, n);
    EXPECT_THROW(mpf_to_bits(f, n), std::logic_error);

    f = 1.125;
    EXPECT_NO_THROW(mpf_to_bits(f, n));

    f = 2;
    EXPECT_THROW(mpf_to_bits(f, n), std::logic_error);
}

//TEST(MpfToBits, CornerCase) {
//    mpf_class x("1.99999", 6, 10);
//    mp_exp_t exp;
//    std::string bits_str = x.get_str(exp, 2, 6 + 6);
//}

TEST(MpzToBits, SimpleChecks) {
    mpz_class x = 1;
    auto bits = mpz_to_bits(x);
    std::vector<NTL::GF2> ans{NTL::GF2{1}};
    EXPECT_EQ(bits.size(), ans.size());
    to_bits_check_aux(bits, ans);

    x = 14;
    bits = mpz_to_bits(x);
    ans = {NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{0}};
    EXPECT_EQ(bits.size(), ans.size());
    to_bits_check_aux(bits, ans);

    x = 0;
    bits = mpz_to_bits(x);
    ans = {NTL::GF2{0}};
    EXPECT_EQ(bits.size(), ans.size());
    to_bits_check_aux(bits, ans);
}

TEST(BitsToMpf, SimpleChecks) {
    std::vector<NTL::GF2> bits{NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{1}};
    int prec = 100;
    mpf_class x = bits_to_mpf(bits, prec);
    mpf_class res("0.125", prec, 10);
    mpf_class err(0.000000001, prec);
    EXPECT_TRUE(abs(x - res) < err);

    bits = {NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{1}};
    x = bits_to_mpf(bits, prec);
    res = 1.125;
    EXPECT_TRUE(abs(x - res) < err);

    bits = {NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{1}, NTL::GF2{1}, NTL::GF2{0}, NTL::GF2{1}};
    x = bits_to_mpf(bits, prec);
    res = 1.40625;
    EXPECT_TRUE(abs(x - res) < err);
}

