#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <limits>
#include <cmath>

using testing::ContainerEq;

namespace {

// ---- Common Foundation: Abstraction of calling conventions and type-agnostic core logic. ----
template <typename T>
class LinspaceTestBase : public ::testing::Test {
protected:
    virtual void test_matlib_linspace(int32_t n, float *x, int32_t incX, float start, float stop)
    {
        rindow_matlib_s_linspace(n, x, incX, start, stop);
    }

    virtual void test_matlib_linspace(int32_t n, double *x, int32_t incX, double start, double stop)
    {
        rindow_matlib_d_linspace(n, x, incX, start, stop);
    }

    

    // A basic case shared by Int and Float types; called from each TYPED_TEST.
    void run_normal_case()
    {
        const int32_t N = 6;
        const int32_t incX = 1;

        T X1[N] = {255, 255, 255, 255, 255, 255};
        this->test_matlib_linspace(N, X1, incX, 0, 5);
        T R1[N] = {0, 1, 2, 3, 4, 5};
        EXPECT_THAT(R1, ContainerEq(X1));

        T X2[N] = {255, 255, 255, 255, 255, 255};
        this->test_matlib_linspace(N, X2, incX, 1, 11);
        T R2[N] = {1, 3, 5, 7, 9, 11};
        EXPECT_THAT(R2, ContainerEq(X2));

        T X3[N] = {255, 255, 255, 255, 255, 255};
        this->test_matlib_linspace(3, X3, 2, 0, 2);
        T R3[N] = {0, 255, 1, 255, 2, 255};
        EXPECT_THAT(R3, ContainerEq(X3));

        T X4[N] = {255, 255, 255, 255, 255, 255};
        this->test_matlib_linspace(N, X4, incX, 6, 1);
        T R4[N] = {6, 5, 4, 3, 2, 1};
        EXPECT_THAT(R4, ContainerEq(X4));
    }
};

// ==================== Integer-only suite ====================
// Not implemented yet.

// ==================== Floating-point type-only suite ====================
template <typename T>
class LinspaceFloatTest : public LinspaceTestBase<T> {
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
TYPED_TEST_SUITE(LinspaceFloatTest, FloatTestTypes);

TYPED_TEST(LinspaceFloatTest, normal) {
    this->run_normal_case();
}

TYPED_TEST(LinspaceFloatTest, with_inf) {
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam value = 0;
    TypeParam inf = this->get_inf(value);

    TypeParam X1[N] = {255, 255, 255, 255, 255, 255};
    this->test_matlib_linspace(N, X1, incX, -1, 1.5);
    TypeParam R1[N] = {-1.0, -0.5, 0, 0.5, 1.0, 1.5};
    EXPECT_THAT(R1, ContainerEq(X1));

    TypeParam X2[N] = {255, 255, 255, 255, 255, 255};
    this->test_matlib_linspace(N, X2, incX, 1.5, -1);
    TypeParam R2[N] = {1.5, 1.0, 0.5, 0, -0.5, -1.0};
    EXPECT_THAT(R2, ContainerEq(X2));

}

}