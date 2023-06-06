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


