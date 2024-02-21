#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"

using testing::ContainerEq;

namespace {

template <typename T>
class ZerosTest : public ::testing::Test {
protected:
    virtual void test_matlib_zeros(int32_t n, float *x, int32_t incX)
    {
        rindow_matlib_s_zeros(n, x, incX);
    }

    virtual void test_matlib_zeros(int32_t n, double *x, int32_t incX)
    {
        rindow_matlib_d_zeros(n, x, incX);
    }

    virtual void test_matlib_zeros(int32_t n, int32_t *x, int32_t incX)
    {
        int32_t dtype = rindow_matlib_dtype_int32;
        rindow_matlib_i_zeros(dtype, n, x, incX);
    }

    virtual void test_matlib_zeros(int32_t n, bool *x, int32_t incX)
    {
        int32_t dtype = rindow_matlib_dtype_bool;
        rindow_matlib_i_zeros(dtype, n, x, incX);
    }

};
typedef ::testing::Types<float, double, int32_t, bool> TestTypes;
TYPED_TEST_SUITE(ZerosTest, TestTypes);

TYPED_TEST(ZerosTest, FillNormal) {
    const int32_t N = 3;
    const int32_t incX = 1;

    TypeParam X[N] = {(TypeParam)99,(TypeParam)99,(TypeParam)99};

    this->test_matlib_zeros(N, X, incX);

    TypeParam R1[N] = {0, 0, 0};
    EXPECT_THAT(R1, ContainerEq(X));
}

}
