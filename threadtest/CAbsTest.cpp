#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {


struct CAbsTestParamFloat {
    using complex_type = matlib_complex_float;
    using real_type = float;

    static void test(
        int32_t n,
        complex_type *x,
        int32_t incX,
        real_type *y,
        int32_t incY)
    {
        rindow_matlib_c_abs(n, x, incX, y, incY);
    }
};

struct CAbsTestParamDouble {
    using complex_type = matlib_complex_double;
    using real_type = double;

    static void test(
        int32_t n,
        complex_type *x,
        int32_t incX,
        real_type *y,
        int32_t incY)
    {
        rindow_matlib_z_abs(n, x, incX, y, incY);
    }
};

using TestTypes = ::testing::Types<
    CAbsTestParamFloat,
    CAbsTestParamDouble
>;

template <typename T>
class CAbsTest : public ::testing::Test {
};

TYPED_TEST_SUITE(CAbsTest, TestTypes);

TYPED_TEST(CAbsTest, normal) {
    using Complex = typename TypeParam::complex_type;
    using Real = typename TypeParam::real_type;

    const int32_t N = 4;
    const int32_t incX = 1;
    const int32_t incY = 1;

    Complex X[N] = {
        {1.0, 0.0},
        {-2.0, 0.0},
        {4.0, 0.0},
        {-9.0, 0.0}
    };

    Real R[N] = {0.0, 0.0, 0.0, 0.0};

    TypeParam::test(N, X, incX, R, incY);

    EXPECT_EQ(R[0], 1.0);
    EXPECT_EQ(R[1], 2.0);
    EXPECT_EQ(R[2], 4.0);
    EXPECT_EQ(R[3], 9.0);
}

TYPED_TEST(CAbsTest, imagvalue) {
    using Complex = typename TypeParam::complex_type;
    using Real = typename TypeParam::real_type;

    const int32_t N = 4;
    const int32_t incX = 1;
    const int32_t incY = 1;

    Complex X[N] = {
        {0.0, 1.0},
        {0.0,-2.0},
        {3.0,-4.0},
        {4.0,-3.0}
    };

    Real R[N] = {0.0, 0.0, 0.0, 0.0};

    TypeParam::test(N, X, incX, R, incY);

    EXPECT_EQ(R[0], 1.0);
    EXPECT_EQ(R[1], 2.0);
    EXPECT_EQ(R[2], 5.0);
    EXPECT_EQ(R[3], 5.0);
}

}