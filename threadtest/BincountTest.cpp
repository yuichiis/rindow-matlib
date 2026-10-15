#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"

using testing::ContainerEq;

namespace {

template <typename T>
class BincountTest : public ::testing::Test {
protected:
    virtual void test_matlib_bincount(
        int32_t trans, int32_t m, int32_t n, int32_t numClasses,
        const int32_t *input, int32_t *output
    ) {
        rindow_matlib_i32_bincount(trans, m, n, numClasses, input, output);
    }
    virtual void test_matlib_bincount(
        int32_t trans, int32_t m, int32_t n, int32_t numClasses,
        const int64_t *input, int64_t *output
    ) {
        rindow_matlib_i64_bincount(trans, m, n, numClasses, input, output);
    }
};
typedef ::testing::Types<int32_t, int64_t> TestTypes;
TYPED_TEST_SUITE(BincountTest, TestTypes);

// axis=-1: input [1,6] -> output [1,4]
TYPED_TEST(BincountTest, once) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t K = 4;

    TypeParam X[M][N] = {{0, 2, 1, 2, 2, 0}};
    TypeParam Y[M][K] = {{9, 9, 9, 9}};  // Confirm that the output is cleared beforehand
    this->test_matlib_bincount(trans, M, N, K, (TypeParam *)X, (TypeParam *)Y);

    TypeParam R1[M][N] = {{0, 2, 1, 2, 2, 0}};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][K] = {{2, 1, 3, 0}};
    EXPECT_THAT(R2, ContainerEq(Y));
}

// axis=-1: input [3,4] -> output [3,3]
TYPED_TEST(BincountTest, axisLast) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 3;
    const int32_t N = 4;
    const int32_t K = 3;

    TypeParam X[M][N] = {
        {0, 1, 1, 2},
        {2, 2, 2, 2},
        {0, 0, 0, 0},
    };
    TypeParam Y[M][K] = {};
    this->test_matlib_bincount(trans, M, N, K, (TypeParam *)X, (TypeParam *)Y);

    TypeParam R1[M][N] = {
        {0, 1, 1, 2},
        {2, 2, 2, 2},
        {0, 0, 0, 0},
    };
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][K] = {
        {1, 2, 1},
        {0, 0, 4},
        {4, 0, 0},
    };
    EXPECT_THAT(R2, ContainerEq(Y));
}

// axis=0: input [4,3] -> output [3,3]
TYPED_TEST(BincountTest, axisFirst) {
    const int32_t trans = RINDOW_MATLIB_TRANS;
    const int32_t M = 4;
    const int32_t N = 3;
    const int32_t K = 3;

    TypeParam X[M][N] = {
        {0, 2, 0},
        {1, 2, 0},
        {1, 2, 0},
        {2, 2, 0},
    };
    TypeParam Y[K][N] = {
        {9, 9, 9},
        {9, 9, 9},
        {9, 9, 9},
    };
    this->test_matlib_bincount(trans, M, N, K, (TypeParam *)X, (TypeParam *)Y);

    TypeParam R1[M][N] = {
        {0, 2, 0},
        {1, 2, 0},
        {1, 2, 0},
        {2, 2, 0},
    };
    EXPECT_THAT(R1, ContainerEq(X));
    // Column 0: {0,1,1,2}->[1,2,1], Column 1: {2,2,2,2}->[0,0,4], Column 2: {0,0,0,0}->[4,0,0]
    TypeParam R2[K][N] = {
        {1, 0, 4},
        {2, 0, 0},
        {1, 4, 0},
    };
    EXPECT_THAT(R2, ContainerEq(Y));
}

// Out-of-range values (negative or >= numClasses) are ignored
TYPED_TEST(BincountTest, outOfRangeIgnored) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 2;
    const int32_t N = 5;
    const int32_t K = 3;

    TypeParam X[M][N] = {
        {-1, 0, 3, 2, 100},
        {1, 1, -5, 3, 1},
    };
    TypeParam Y[M][K] = {};
    this->test_matlib_bincount(trans, M, N, K, (TypeParam *)X, (TypeParam *)Y);

    TypeParam R2[M][K] = {
        {1, 0, 1},
        {0, 3, 0},
    };
    EXPECT_THAT(R2, ContainerEq(Y));
}

// Out-of-range values are also ignored for axis=0
TYPED_TEST(BincountTest, outOfRangeIgnoredAxisFirst) {
    const int32_t trans = RINDOW_MATLIB_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;
    const int32_t K = 2;

    TypeParam X[M][N] = {
        {-1, 0},
        {1, 2},
        {5, 1},
    };
    TypeParam Y[K][N] = {};
    this->test_matlib_bincount(trans, M, N, K, (TypeParam *)X, (TypeParam *)Y);

    // Column 0: {-1,1,5} -> [0,1], Column 1: {0,2,1} -> [1,1]
    TypeParam R2[K][N] = {
        {0, 1},
        {1, 1},
    };
    EXPECT_THAT(R2, ContainerEq(Y));
}

// When numClasses is larger than the maximum value, the tail is 0
TYPED_TEST(BincountTest, numClassesLargerThanMax) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 1;
    const int32_t N = 3;
    const int32_t K = 6;

    TypeParam X[M][N] = {{1, 1, 2}};
    TypeParam Y[M][K] = {{9, 9, 9, 9, 9, 9}};
    this->test_matlib_bincount(trans, M, N, K, (TypeParam *)X, (TypeParam *)Y);

    TypeParam R2[M][K] = {{0, 2, 1, 0, 0, 0}};
    EXPECT_THAT(R2, ContainerEq(Y));
}

// Compare with sequential calculation results using a larger size to exercise the parallel path (axis=-1)
TYPED_TEST(BincountTest, largeAxisLast) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 1000;
    const int32_t N = 200;
    const int32_t K = 7;

    std::vector<TypeParam> X(M * N);
    for(int32_t i = 0; i < M * N; i++) {
        X[i] = (TypeParam)((i * 31 + i / N) % K);
    }
    std::vector<TypeParam> Y(M * K, 9);
    std::vector<TypeParam> expected(M * K, 0);
    for(int32_t i = 0; i < M; i++) {
        for(int32_t j = 0; j < N; j++) {
            expected[i * K + X[i * N + j]]++;
        }
    }
    this->test_matlib_bincount(trans, M, N, K, X.data(), Y.data());
    EXPECT_THAT(Y, ContainerEq(expected));
}

// Compare with sequential calculation results using a larger size to exercise the parallel path (axis=0)
TYPED_TEST(BincountTest, largeAxisFirst) {
    const int32_t trans = RINDOW_MATLIB_TRANS;
    const int32_t M = 200;
    const int32_t N = 1000;
    const int32_t K = 7;

    std::vector<TypeParam> X(M * N);
    for(int32_t i = 0; i < M * N; i++) {
        X[i] = (TypeParam)((i * 17 + i / N) % K);
    }
    std::vector<TypeParam> Y(K * N, 9);
    std::vector<TypeParam> expected(K * N, 0);
    for(int32_t i = 0; i < M; i++) {
        for(int32_t j = 0; j < N; j++) {
            expected[X[i * N + j] * N + j]++;
        }
    }
    this->test_matlib_bincount(trans, M, N, K, X.data(), Y.data());
    EXPECT_THAT(Y, ContainerEq(expected));
}

}