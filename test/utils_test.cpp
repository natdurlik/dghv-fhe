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

