#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"

using testing::ContainerEq;

namespace {

template <typename T>
class MaskingTest : public ::testing::Test {
protected:
    virtual void test_matlib_masking(
        int32_t m,int32_t n,int32_t k,
        float fill,
        uint8_t *x,
        float *a
    ) {
        rindow_matlib_s_masking(m,n,k,fill,x,a);
    }
    virtual void test_matlib_masking(
        int32_t m,int32_t n,int32_t k,
        double fill,
        uint8_t *x,
        double *a
    ) {
        rindow_matlib_d_masking(m,n,k,fill,x,a);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(MaskingTest, TestTypes);

TYPED_TEST(MaskingTest, simple) {
    const int32_t M = 1;
    const int32_t N = 1;
    const int32_t K = 6;

    // rows<cols
    uint8_t X[M][K] = {{true,true,false,false,false,true}};
    TypeParam A[M][N][K] = {{{1,10,100,-1,-10,-100}}};
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        (TypeParam)0.0, // fill
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {{true,true,false,false,false,true}};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K] = {{{1,10,  0, 0,  0,-100}}};
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, broadcast_inner_mGTn) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;
    const int32_t K = 1;

    // m>n
    uint8_t X[M][K] = {{true},{false},{true}};
    TypeParam A[M][N][K] = {
        {{ 11},{-12}},
        {{-21},{ 22}},
        {{ 31},{-32}}
    };
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        (TypeParam)0.0, // fill
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {{true},{false},{true}};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K] = {
        {{ 11},{-12}},
        {{  0},{  0}},
        {{ 31},{-32}}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, broadcast_inner_mLTn) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 2;
    const int32_t N = 3;
    const int32_t K = 1;

    // m<n
    uint8_t X[M][K] = {{true},{false}};
    TypeParam A[M][N][K] = {
        {{ 11},{ 12},{-13}},
        {{-21},{ 22},{ 23}}
    };
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        (TypeParam)0,   // fill
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {{true},{false}};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K] = {
        {{ 11},{ 12},{-13}},
        {{  0},{  0},{  0}}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, broadcast_outer_nGTk) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 1;
    const int32_t N = 3;
    const int32_t K = 2;

    // n<k
    uint8_t X[M][K] = {{true,false}};
    TypeParam A[M][N][K] = {
        {{ 11,-12},
         {-21, 22},
         { 31,-32}}
    };
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        (TypeParam)0.0, // fill
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {{true,false}};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K] = {
        {{ 11,  0},
         {-21,  0},
         { 31,  0}}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, broadcast_outer_nLTk) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 1;
    const int32_t N = 2;
    const int32_t K = 3;

    // n<k
    uint8_t X[M][K] = {{true,false,true}};
    TypeParam A[M][N][K] = {
        {{ 11, 12,-13},
         {-21, 22, 23}}
    };
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        (TypeParam)0.0, // fill
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {{true,false,true}};
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K] = {
        {{ 11,  0,-13},
         {-21,  0, 23}}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, broadcast_both_mGTn) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;
    const int32_t K = 4;

    // n<k
    uint8_t X[M][K] = {
        {true,false,false,false},
        {true,true, false,false},
        {true,true, true, false}
    };
    TypeParam A[M][N][K] = {
        {{ 111, 112,-113, 114},{-121, 122, 123, 124}},
        {{ 211, 212,-213, 214},{-221, 222, 223, 224}},
        {{ 311, 312,-313, 314},{-321, 322, 323, 324}}
    };
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        (TypeParam)0.0, // fill
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {
        {true,false,false,false},
        {true,true, false,false},
        {true,true, true, false}
    };
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K] = {
        {{ 111,   0,   0,   0},{-121,   0,   0,   0}},
        {{ 211, 212,   0,   0},{-221, 222,   0,   0}},
        {{ 311, 312,-313,   0},{-321, 322, 323,   0}}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}

TYPED_TEST(MaskingTest, broadcast_both_mLTn) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 2;
    const int32_t N = 3;
    const int32_t K = 4;

    // n<k
    uint8_t X[M][K] = {
        {true,true, false,false},
        {true,true, true, false}
    };
    TypeParam A[M][N][K] = {
        {{ 111, 112,-113, 114},{-121, 122, 123, 124},{-131, 132, 133, 134}},
        {{ 211, 212,-213, 214},{-221, 222, 223, 224},{-231, 232, 233, 234}}
    };
    this->test_matlib_masking(
        M,              // int32_t m,
        N,              // int32_t n,
        K,              // int32_t k,
        (TypeParam)0.0, // fill
        (uint8_t*)X,    // *x,
        (TypeParam *)A  // *a,
    );
    uint8_t R1[M][K] = {
        {true,true, false,false},
        {true,true, true, false}
    };
    EXPECT_THAT(R1, ContainerEq(X));
    TypeParam R2[M][N][K] = {
        {{ 111, 112,   0,   0},{-121, 122,   0,   0},{-131, 132,   0,   0}},
        {{ 211, 212,-213,   0},{-221, 222, 223,   0},{-231, 232, 233,   0}}
    };
    EXPECT_THAT(R2, ContainerEq(A));
}

}
