#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdbool.h>
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class GreaterEqualTest : public ::testing::Test {
protected:
    virtual void test_matlib_greater_equal(
        int32_t m,int32_t n,float *a, int32_t ldA,float *x, int32_t incX)
    {
        rindow_matlib_s_greater_equal(m,n,a,ldA,x,incX);
    }
    virtual void test_matlib_greater_equal(
        int32_t m,int32_t n,double *a, int32_t ldA,double *x, int32_t incX)
    {
        rindow_matlib_d_greater_equal(m,n,a,ldA,x,incX);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(GreaterEqualTest, TestTypes);

TYPED_TEST(GreaterEqualTest, GreaterEqualNormal) {
    const int32_t M = 2;
    const int32_t N = 3;
    
    TypeParam X[N] =    {-2,1,2};
    TypeParam A[M][N] = {{-1,0,4}, {2,0,2}};
    test_matlib_greater_equal(
        M,              // int32_t m,
        N,              // int32_t n,
        (TypeParam *)A, // *a,
        N,              // int32_t ldA
        (TypeParam *)X, // *x,
        1               // int32_t incX,
    );
    TypeParam R1[N] = {-2,1,2};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N] = {{1,0,1},{1,0,1}};
    EXPECT_THAT(R2, ContainerEq(A));
}
TYPED_TEST(GreaterEqualTest, GreaterEqualNormal2) {
    const int32_t M = 3;
    const int32_t N = 2;
    
    TypeParam X[N] =    {-1,1};
    TypeParam A[M][N] = {{-2,0}, {-2,2}, {0,2}};
    test_matlib_greater_equal(
        M,              // int32_t m,
        N,              // int32_t n,
        (TypeParam *)A, // *a,
        N,              // int32_t ldA
        (TypeParam *)X, // *x,
        1               // int32_t incX,
    );
    TypeParam R1[N] = {-1,1};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N] = {{0,0},{0,1},{1,1}};
    EXPECT_THAT(R2, ContainerEq(A));
}

}
