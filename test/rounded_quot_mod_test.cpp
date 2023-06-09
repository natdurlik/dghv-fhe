#include <gtest/gtest.h>
#include "rounded_quot_mod.h"

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


class IsEvenTest : public ::testing::TestWithParam<std::tuple<std::string, bool>> {
};

TEST_P(IsEvenTest, BasicCases) {
    const auto &param = GetParam();
    mpz_class x(std::get<0>(param));
    bool res = is_even(x);

    EXPECT_EQ(res, std::get<1>(param));
}

INSTANTIATE_TEST_SUITE_P(SmallInts, IsEvenTest, ::testing::Values(
        std::tuple{"0", true},
        std::tuple{"2", true},
        std::tuple{"-10", true},
        std::tuple{"1", false},
        std::tuple{"-1", false},
        std::tuple{"-33", false}
));

TEST(PowOfTwo, SimpleCheck) {
    mpz_class x = pow_of_two(2);
    EXPECT_EQ(x, 4);
    x = pow_of_two(3);
    EXPECT_EQ(x, 8);
    x = pow_of_two(10);
    EXPECT_EQ(x, 1024);
}

