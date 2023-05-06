#include <gtest/gtest.h>
#include <gmpxx.h>
#include "rounded_quot_mod.h"

class QuotTest : public ::testing::TestWithParam<std::tuple<std::string, std::string, int>> {
};

TEST_P(QuotTest, BasicCases) {
    const auto& param = GetParam();
    mpz_class a(std::get<0>(param));
    mpz_class b(std::get<1>(param));
    mpz_class res = quot(a, b);

    EXPECT_EQ(res, std::get<2>(param));
}

INSTANTIATE_TEST_SUITE_P(SmallInts, QuotTest, ::testing::Values(
                std::tuple{"7","2",4},
                std::tuple{"3","6",1},
                std::tuple{"10","1",10},
                std::tuple{"1","10",0},
                std::tuple{"1","2",1}
            ));

class RemTest : public ::testing::TestWithParam<std::tuple<std::string, std::string, int>> {
};

TEST_P(RemTest, BasicCases) {
    const auto& param = GetParam();
    mpz_class a(std::get<0>(param));
    mpz_class b(std::get<1>(param));
    mpz_class res = rem(a, b);

    EXPECT_EQ(res, std::get<2>(param));
}

INSTANTIATE_TEST_SUITE_P(SmallInts, RemTest, ::testing::Values(
                std::tuple{"7","2",-1},
                std::tuple{"3","6",-3},
                std::tuple{"10","1",0},
                std::tuple{"1","10",1},
                std::tuple{"1","2",-1}
            ));
