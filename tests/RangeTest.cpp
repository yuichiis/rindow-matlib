#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <limits>
#include <cmath>

using testing::ContainerEq;

namespace {

// ---- Common Foundation: Abstraction of calling conventions and type-agnostic core logic. ----
template <typename T>
class RangeTestBase : public ::testing::Test {
protected:
    virtual void test_matlib_range(int32_t n, float *x, int32_t incX, float start, float limit, float delta)
    {
        rindow_matlib_s_range(n, x, incX, start, limit, delta);
    }

    virtual void test_matlib_range(int32_t n, double *x, int32_t incX, double start, double limit, double delta)
    {
        rindow_matlib_d_range(n, x, incX, start, limit, delta);
    }

    virtual void test_matlib_range(int32_t n, int32_t *x, int32_t incX, int64_t start, int64_t limit, int64_t delta)
    {
        int32_t dtype = rindow_matlib_dtype_int32;
        rindow_matlib_i_range(dtype, n, x, incX, start, limit, delta);
    }

    virtual void test_matlib_range(int32_t n, int64_t *x, int32_t incX, int64_t start, int64_t limit, int64_t delta)
    {
        int32_t dtype = rindow_matlib_dtype_int64;
        rindow_matlib_i_range(dtype, n, x, incX, start, limit, delta);
    }

    virtual void test_matlib_range(int32_t n, uint8_t *x, int32_t incX, int64_t start, int64_t limit, int64_t delta)
    {
        int32_t dtype = rindow_matlib_dtype_uint8;
        rindow_matlib_i_range(dtype, n, x, incX, start, limit, delta);
    }

    // A basic case shared by Int and Float types; called from each TYPED_TEST.
    void run_normal_case()
    {
        const int32_t N = 6;
        const int32_t incX = 1;

        T X1[N] = {255, 255, 255, 255, 255, 255};
        this->test_matlib_range(N, X1, incX, 0, 6, 1);
        T R1[N] = {0, 1, 2, 3, 4, 5};
        EXPECT_THAT(R1, ContainerEq(X1));

        T X2[N] = {255, 255, 255, 255, 255, 255};
        this->test_matlib_range(N, X2, incX, 1, 12, 2);
        T R2[N] = {1, 3, 5, 7, 9, 11};
        EXPECT_THAT(R2, ContainerEq(X2));

        T X3[N] = {255, 255, 255, 255, 255, 255};
        this->test_matlib_range(3, X3, 2, 0, 3, 1);
        T R3[N] = {0, 255, 1, 255, 2, 255};
        EXPECT_THAT(R3, ContainerEq(X3));

        T X4[N] = {255, 255, 255, 255, 255, 255};
        this->test_matlib_range(N, X4, incX, 6, 0, -1);
        T R4[N] = {6, 5, 4, 3, 2, 1};
        EXPECT_THAT(R4, ContainerEq(X4));
    }
};

// ==================== Integer-only suite ====================
template <typename T>
class RangeIntTest : public RangeTestBase<T> {};

typedef ::testing::Types<int32_t, int64_t, uint8_t> IntTestTypes;
TYPED_TEST_SUITE(RangeIntTest, IntTestTypes);

TYPED_TEST(RangeIntTest, normal) {
    this->run_normal_case();

    const int32_t N = 6;
    const int32_t incX = 1;

    TypeParam X5[N] = {255, 255, 255, 255, 255, 255};
    this->test_matlib_range(N, X5, incX, 0, 4, 1);
    TypeParam R5[N] = {0, 1, 2, 3, 3, 3};
    EXPECT_THAT(R5, ContainerEq(X5));

    TypeParam X6[N] = {255, 255, 255, 255, 255, 255};
    this->test_matlib_range(N, X6, incX, 6, 2, -1);
    TypeParam R6[N] = {6, 5, 4, 3, 3, 3};
    EXPECT_THAT(R6, ContainerEq(X6));
}

// ==================== Floating-point type-only suite ====================
template <typename T>
class RangeFloatTest : public RangeTestBase<T> {
protected:
    virtual float get_inf(float value)
    {
        return std::numeric_limits<float>::infinity();
    }

    virtual double get_inf(double value)
    {
        return std::numeric_limits<double>::infinity();
    }

    virtual float get_nan(float value)
    {
        return NAN;
    }

    virtual double get_nan(double value)
    {
        return NAN;
    }
};

typedef ::testing::Types<float, double> FloatTestTypes;
TYPED_TEST_SUITE(RangeFloatTest, FloatTestTypes);

TYPED_TEST(RangeFloatTest, normal) {
    this->run_normal_case();
}

TYPED_TEST(RangeFloatTest, with_inf) {
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam value = 0;
    TypeParam inf = this->get_inf(value);

    TypeParam X1[N] = {255, 255, 255, 255, 255, 255};
    this->test_matlib_range(N, X1, incX, 0, 3, 0.5);
    TypeParam R1[N] = {0, 0.5, 1.0, 1.5, 2.0, 2.5};
    EXPECT_THAT(R1, ContainerEq(X1));

    TypeParam X2[N] = {255, 255, 255, 255, 255, 255};
    this->test_matlib_range(N, X2, incX, 0, inf, 0.5);
    TypeParam R2[N] = {0, 0.5, 1.0, 1.5, 2.0, 2.5};
    EXPECT_THAT(R2, ContainerEq(X2));


    TypeParam X5[N] = {255, 255, 255, 255, 255, 255};
    this->test_matlib_range(N, X5, incX, 0, 4, 1);
    TypeParam R5[N] = {0, 1, 2, 3, 3, 3};
    R5[4] = R5[5] = std::nextafter((TypeParam)4, -std::numeric_limits<TypeParam>::infinity());
    EXPECT_THAT(R5, ContainerEq(X5));

    TypeParam X6[N] = {255, 255, 255, 255, 255, 255};
    this->test_matlib_range(N, X6, incX, 6, 2, -1);
    TypeParam R6[N] = {6, 5, 4, 3, 3, 3};
    R6[4] = R6[5] = std::nextafter((TypeParam)2, std::numeric_limits<TypeParam>::infinity());
    EXPECT_THAT(R6, ContainerEq(X6));
}

}