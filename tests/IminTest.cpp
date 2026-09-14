#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <limits>
#include <cmath>

using testing::ContainerEq;

namespace {

// ---- Common Foundation: Abstraction of calling conventions and type-agnostic core logic. ----
template <typename T>
class IminTestBase : public ::testing::Test {
protected:
    virtual int32_t test_matlib_imin(int32_t n,float *x, int32_t incX)
    {
        return rindow_matlib_s_imin(n,x, incX);
    }

    virtual int32_t test_matlib_imin(int32_t n,double *x, int32_t incX)
    {
        return rindow_matlib_d_imin(n,x, incX);
    }

    virtual int32_t test_matlib_imin(int32_t n,int32_t *x, int32_t incX)
    {
        int32_t dtype = rindow_matlib_dtype_int32;
        return rindow_matlib_i_imin(dtype, n, x, incX);
    }

    // A basic case shared by Int and Float types; called from each TYPED_TEST.
    void run_normal_case()
    {
        const int32_t N = 6;
        const int32_t incX = 1;
        T X[N] = {-1, 2,-3,-4, 5,-6};

        int32_t res = this->test_matlib_imin(N, X, incX);
        EXPECT_EQ(5, res);
    }
};

// ==================== Integer-only suite ====================
template <typename T>
class IminIntTest : public IminTestBase<T> {};

typedef ::testing::Types<int32_t> IntTestTypes;
TYPED_TEST_SUITE(IminIntTest, IntTestTypes);

TYPED_TEST(IminIntTest, normal) {
    this->run_normal_case();
}

// ==================== Floating-point type-only suite ====================
template <typename T>
class IminFloatTest : public IminTestBase<T> {
protected:
    virtual float get_inf(float value)
    {
        return std::numeric_limits<float>::infinity();
    }

    virtual double get_inf(double value)
    {
        return std::numeric_limits<double>::infinity();
    }

    virtual int32_t get_inf(int32_t value)
    {
        return INT_MAX;
    }

    virtual float get_nan(float value)
    {
        return NAN;
    }

    virtual double get_nan(double value)
    {
        return NAN;
    }

    virtual int32_t get_nan(int32_t value)
    {
        return INT_MAX;
    }
};

typedef ::testing::Types<float, double> FloatTestTypes;
TYPED_TEST_SUITE(IminFloatTest, FloatTestTypes);

TYPED_TEST(IminFloatTest, normal) {
    this->run_normal_case();
}

TYPED_TEST(IminFloatTest, with_inf) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam value=0;
    TypeParam inf = this->get_inf(value);
    TypeParam nan = this->get_nan(value);

    TypeParam X[N] = {-1, 2,inf,-4, 5,-6};
    int32_t res1 = this->test_matlib_imin(N, X, incX);
    EXPECT_EQ(5, res1);

    TypeParam X2[N] = {-1, -inf,-3,-4, 5,-6};
    int32_t res2 = this->test_matlib_imin(N, X2, incX);
    EXPECT_EQ(1, res2);

    TypeParam X3[N] = {-inf, -inf,-inf,-inf, -inf,-inf};
    int32_t res3 = this->test_matlib_imin(N, X3, incX);
    EXPECT_EQ(0, res3);
}

TYPED_TEST(IminFloatTest, with_nan) {
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam value = 0;
    TypeParam nan = this->get_nan(value);

    TypeParam X4[N] = {-1, 2,-3, nan, -4, 5};
    int32_t res4 = this->test_matlib_imin(N, X4, incX);
    EXPECT_EQ(3, res4);
}

}
