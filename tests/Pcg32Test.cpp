#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdlib.h>
#include <numeric>

using testing::ContainerEq;

namespace {

TEST(Pcg32Test, random) {
    const int64_t seed = 12345;
    const int64_t sequence = 1;

    int64_t rng[2];
    rindow_matlib_pcg32srand(rng, seed, sequence);

    uint32_t r1 = rindow_matlib_pcg32rand(rng);
    uint32_t r2 = rindow_matlib_pcg32rand(rng);
    uint32_t r3 = rindow_matlib_pcg32rand(rng);

    EXPECT_NE(r1, r2);
    EXPECT_NE(r2, r3);
    EXPECT_NE(r1, r3);

    rindow_matlib_pcg32srand(rng, seed, sequence);

    uint32_t r1_2 = rindow_matlib_pcg32rand(rng);
    uint32_t r2_2 = rindow_matlib_pcg32rand(rng);
    uint32_t r3_2 = rindow_matlib_pcg32rand(rng);

    EXPECT_EQ(r1, r1_2);
    EXPECT_EQ(r2, r2_2);
    EXPECT_EQ(r3, r3_2);

}

TEST(Pcg32Test, random_int32) {
    const int64_t seed = 12345;
    const int64_t sequence = 1;
    const int32_t min = -100;
    const int32_t max = 100;

    int64_t rng[2];
    rindow_matlib_pcg32srand(rng, seed, sequence);

    int32_t r1 = rindow_matlib_pcg32randint32(rng, min, max);
    int32_t r2 = rindow_matlib_pcg32randint32(rng, min, max);
    int32_t r3 = rindow_matlib_pcg32randint32(rng, min, max);

    EXPECT_NE(r1, r2);
    EXPECT_NE(r2, r3);
    EXPECT_NE(r1, r3);

    EXPECT_GE(r1, min);
    EXPECT_GE(r2, min);
    EXPECT_GE(r3, min);
    EXPECT_LE(r1, max);
    EXPECT_LE(r2, max);
    EXPECT_LE(r3, max);

    rindow_matlib_pcg32srand(rng, seed, sequence);

    int32_t r1_2 = rindow_matlib_pcg32randint32(rng, min, max);
    int32_t r2_2 = rindow_matlib_pcg32randint32(rng, min, max);
    int32_t r3_2 = rindow_matlib_pcg32randint32(rng, min, max);

    EXPECT_EQ(r1, r1_2);
    EXPECT_EQ(r2, r2_2);
    EXPECT_EQ(r3, r3_2);

}

TEST(Pcg32Test, random_int64) {
    const int64_t seed = 12345;
    const int64_t sequence = 1;
    const int64_t min = -100;
    const int64_t max = 100;

    int64_t rng[2];
    rindow_matlib_pcg32srand(rng, seed, sequence);

    int64_t r1 = rindow_matlib_pcg32randint64(rng, min, max);
    int64_t r2 = rindow_matlib_pcg32randint64(rng, min, max);
    int64_t r3 = rindow_matlib_pcg32randint64(rng, min, max);

    EXPECT_NE(r1, r2);
    EXPECT_NE(r2, r3);
    EXPECT_NE(r1, r3);

    EXPECT_GE(r1, min);
    EXPECT_GE(r2, min);
    EXPECT_GE(r3, min);
    EXPECT_LE(r1, max);
    EXPECT_LE(r2, max);
    EXPECT_LE(r3, max);

    rindow_matlib_pcg32srand(rng, seed, sequence);

    int64_t r1_2 = rindow_matlib_pcg32randint64(rng, min, max);
    int64_t r2_2 = rindow_matlib_pcg32randint64(rng, min, max);
    int64_t r3_2 = rindow_matlib_pcg32randint64(rng, min, max);

    EXPECT_EQ(r1, r1_2);
    EXPECT_EQ(r2, r2_2);
    EXPECT_EQ(r3, r3_2);

}

}
