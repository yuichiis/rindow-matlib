#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdbool.h>

using testing::ContainerEq;

namespace {

template <typename T>
class AddTest : public ::testing::Test {
protected:
    virtual void test_matlib_add(
        int32_t trans, int32_t m, int32_t n,
        float alpha, float *x, int32_t incX, float *a, int32_t ldA)
    {
        rindow_matlib_s_add(trans, m, n, alpha, x, incX, a, ldA);
    }
    virtual void test_matlib_add(
        int32_t trans, int32_t m, int32_t n,
        double alpha, double *x, int32_t incX, double *a, int32_t ldA)
    {
        rindow_matlib_d_add(trans, m, n, alpha, x, incX, a, ldA);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(AddTest, TestTypes);

TYPED_TEST(AddTest, AddNormal) {
    const int32_t M = 2;
    const int32_t N = 3;
    // float
    TypeParam X[N] = {1, 2, 3};
    TypeParam Y[M][N] = {{1,10,100}, {-1,-10,-100}};
    test_matlib_add(
        (int32_t)RINDOW_MATLIB_NO_TRANS, // int32_t trans,
        M,              // int32_t m,
        N,              // int32_t n,
        (TypeParam)2.0, // alpha,
        (TypeParam *)X, // *x,
        (int32_t)1,              // int32_t incX,
        (TypeParam *)Y, // *a,
        N               // int32_t ldA
    );
    TypeParam R1[N] = {1,2,3};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N] = {{3,14,106}, {1,-6,-94}};
    EXPECT_THAT(R2, ContainerEq(Y));
}

TYPED_TEST(AddTest, AddTrans) {
    const int32_t M = 2;
    const int32_t N = 3;
    // float
    TypeParam X[N] = {1, 2, 3};
    TypeParam Y[N][M] = {{1,10}, {100,-1}, {-10,-100}};
    test_matlib_add(
        RINDOW_MATLIB_TRANS, // int32_t trans,
        N,              // int32_t m,
        M,              // int32_t n,
        (TypeParam)1.0, // alpha,
        (TypeParam *)X, // *x,
        1,              // int32_t incX,
        (TypeParam *)Y, // *a,
        M               // int32_t ldA
    );
    TypeParam R1[N] = {1,2,3};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[N][M] = {{2,11},{102,1},{-7,-97}};
    EXPECT_THAT(R2, ContainerEq(Y));
}
}
