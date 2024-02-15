#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"

using testing::ContainerEq;

namespace {

template <typename T>
class DuplicateTest : public ::testing::Test {
protected:
    virtual void test_matlib_duplicate(
        int32_t trans,int32_t m,int32_t n,
        float *x, int32_t incX,
        float *a, int32_t ldA)
    {
        rindow_matlib_s_duplicate(trans,m,n,x,incX,a,ldA);
    }

    virtual void test_matlib_duplicate(
        int32_t trans,int32_t m,int32_t n,
        double *x, int32_t incX,
        double *a, int32_t ldA)
    {
        rindow_matlib_d_duplicate(trans,m,n,x,incX,a,ldA);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(DuplicateTest, TestTypes);

TYPED_TEST(DuplicateTest, DuplicateOnce) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t repeats = 1;
    const int32_t M = repeats;
    const int32_t N = 6;
    const int32_t incX = 1;
    const int32_t ldA = N;
    TypeParam X[N] = {1,2,3,4,5,6,};
    TypeParam A[M*N];

    this->test_matlib_duplicate(trans,M,N,X,incX,A,ldA);

    TypeParam R1[N] = {1,2,3,4,5,6,};
    EXPECT_THAT(R1, ContainerEq(X));

    TypeParam R2[M*N] = {
        1,2,3,4,5,6,
    };
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(DuplicateTest, DuplicateTwice) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t repeats = 2;
    const int32_t M = repeats;
    const int32_t N = 6;
    const int32_t incX = 1;
    const int32_t ldA = N;
    TypeParam X[N] = {1,2,3,4,5,6,};
    TypeParam A[M*N];

    this->test_matlib_duplicate(trans,M,N,X,incX,A,ldA);

    TypeParam R1[N] = {1,2,3,4,5,6,};
    EXPECT_THAT(R1, ContainerEq(X));

    TypeParam R2[M*N] = {
        1,2,3,4,5,6,
        1,2,3,4,5,6,
    };
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(DuplicateTest, DuplicateTranspose) {
    const int32_t trans = RINDOW_MATLIB_TRANS;
    const int32_t repeats = 2;
    const int32_t M = repeats;
    const int32_t N = 6;
    const int32_t incX = 1;
    const int32_t ldA = M;
    TypeParam X[N] = {1,2,3,4,5,6,};
    TypeParam A[N*M];

    this->test_matlib_duplicate(trans,N,M,X,incX,A,ldA);

    TypeParam R1[N] = {1,2,3,4,5,6,};
    EXPECT_THAT(R1, ContainerEq(X));

    TypeParam R2[N*M] = {
        1,1,
        2,2,
        3,3,
        4,4,
        5,5,
        6,6,
    };
    EXPECT_THAT(R2, ContainerEq(A));
}
}
