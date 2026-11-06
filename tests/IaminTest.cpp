#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <limits>
#include <cmath>

using testing::ContainerEq;

namespace {

// ---- Real types ----
template <typename T>
class IaminRealTest : public ::testing::Test {
protected:
    virtual int32_t test_matlib_iamin(int32_t n, float *x, int32_t incX)
    {
        return rindow_matlib_s_iamin(n, x, incX);
    }

    virtual int32_t test_matlib_iamin(int32_t n, double *x, int32_t incX)
    {
        return rindow_matlib_d_iamin(n, x, incX);
    }
};

typedef ::testing::Types<float, double> RealTestTypes;
TYPED_TEST_SUITE(IaminRealTest, RealTestTypes);

TYPED_TEST(IaminRealTest, normal) {
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam X[N] = {-1, 2, -3, -4, 5, -6};

    int32_t res = this->test_matlib_iamin(N, X, incX);
    EXPECT_EQ(0, res);

    TypeParam Y[N] = {3, -2, -3, -4, 5, -6};
    int32_t res2 = this->test_matlib_iamin(N, Y, incX);
    EXPECT_EQ(1, res2);
}

TYPED_TEST(IaminRealTest, stride) {
    const int32_t N = 6;
    TypeParam X[N] = {4, 100, -2, 100, 5, 100};

    int32_t res = this->test_matlib_iamin(3, X, 2);
    EXPECT_EQ(1, res);

    int32_t res2 = this->test_matlib_iamin(2, X, 3);
    EXPECT_EQ(0, res2);
}

TYPED_TEST(IaminRealTest, with_inf) {
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam inf = std::numeric_limits<TypeParam>::infinity();

    TypeParam X[N] = {3, -2, inf, -4, 5, -6};
    int32_t res1 = this->test_matlib_iamin(N, X, incX);
    EXPECT_EQ(1, res1);

    TypeParam X2[N] = {-1, -inf, -3, -4, 5, -6};
    int32_t res2 = this->test_matlib_iamin(N, X2, incX);
    EXPECT_EQ(0, res2);

    TypeParam X3[N] = {inf, inf, inf, inf, inf, inf};
    int32_t res3 = this->test_matlib_iamin(N, X3, incX);
    EXPECT_EQ(0, res3);

    TypeParam X4[N] = {-inf, -inf, -inf, -inf, -inf, -inf};
    int32_t res4 = this->test_matlib_iamin(N, X4, incX);
    EXPECT_EQ(0, res4);
}

TYPED_TEST(IaminRealTest, with_nan) {
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam nan = std::numeric_limits<TypeParam>::quiet_NaN();

    TypeParam X[N] = {-1, 2, -3, nan, -4, 5};
    int32_t res1 = this->test_matlib_iamin(N, X, incX);
    EXPECT_EQ(3, res1);

    TypeParam X2[N] = {nan, 2, -3, -4, 5, -6};
    int32_t res2 = this->test_matlib_iamin(N, X2, incX);
    EXPECT_EQ(0, res2);
}

TYPED_TEST(IaminRealTest, empty) {
    TypeParam X[1] = {1};

    int32_t res = this->test_matlib_iamin(0, X, 1);
    EXPECT_EQ(0, res);
}

// ---- Complex types ----
struct IaminComplexFloatParam {
    using complex_type = matlib_complex_float;
    using real_type = float;

    static int32_t test(
        int32_t n,
        complex_type *x,
        int32_t incX)
    {
        return rindow_matlib_c_iamin(n, x, incX);
    }
};

struct IaminComplexDoubleParam {
    using complex_type = matlib_complex_double;
    using real_type = double;

    static int32_t test(
        int32_t n,
        complex_type *x,
        int32_t incX)
    {
        return rindow_matlib_z_iamin(n, x, incX);
    }
};

using ComplexTestTypes = ::testing::Types<
    IaminComplexFloatParam,
    IaminComplexDoubleParam
>;

template <typename T>
class IaminComplexTest : public ::testing::Test {
};

TYPED_TEST_SUITE(IaminComplexTest, ComplexTestTypes);

TYPED_TEST(IaminComplexTest, normal) {
    using Complex = typename TypeParam::complex_type;

    const int32_t N = 4;
    const int32_t incX = 1;

    Complex X[N] = {
        {3.0, 4.0},
        {0.0,-1.0},
        {-2.0, 0.0},
        {0.0, 0.0}
    };

    int32_t res = TypeParam::test(N, X, incX);
    EXPECT_EQ(3, res);

    Complex Y[N] = {
        {3.0, 4.0},
        {0.0,-1.0},
        {-2.0, 0.0},
        {1.0, 1.0}
    };

    int32_t res2 = TypeParam::test(N, Y, incX);
    EXPECT_EQ(1, res2);
}

TYPED_TEST(IaminComplexTest, stride) {
    using Complex = typename TypeParam::complex_type;

    const int32_t N = 7;
    Complex X[N] = {
        {6.0, 8.0},
        {100.0, 100.0},
        {0.0, 3.0},
        {100.0, 100.0},
        {-4.0, 0.0},
        {100.0, 100.0},
        {0.0, 0.0}
    };

    int32_t res = TypeParam::test(3, X, 2);
    EXPECT_EQ(1, res);

    int32_t res2 = TypeParam::test(2, X, 3);
    EXPECT_EQ(0, res2);
}

TYPED_TEST(IaminComplexTest, with_inf) {
    using Complex = typename TypeParam::complex_type;
    using Real = typename TypeParam::real_type;

    const int32_t N = 4;
    const int32_t incX = 1;
    Real inf = std::numeric_limits<Real>::infinity();

    Complex X[N] = {
        {3.0, 4.0},
        {0.0,-1.0},
        {inf, 0.0},
        {-2.0, 0.0}
    };

    int32_t res = TypeParam::test(N, X, incX);
    EXPECT_EQ(1, res);

    Complex Y[N] = {
        {inf, 0.0},
        {0.0, inf},
        {inf, inf},
        {-inf, 0.0}
    };

    int32_t res2 = TypeParam::test(N, Y, incX);
    EXPECT_EQ(0, res2);
}

TYPED_TEST(IaminComplexTest, with_nan) {
    using Complex = typename TypeParam::complex_type;
    using Real = typename TypeParam::real_type;

    const int32_t N = 4;
    const int32_t incX = 1;
    Real nan = std::numeric_limits<Real>::quiet_NaN();

    Complex X[N] = {
        {3.0, 4.0},
        {-1.0, 0.0},
        {0.0, nan},
        {-2.0, 0.0}
    };

    int32_t res = TypeParam::test(N, X, incX);
    EXPECT_EQ(2, res);

    Complex Y[N] = {
        {nan, 0.0},
        {-1.0, 0.0},
        {-2.0, 0.0},
        {3.0, 4.0}
    };

    int32_t res2 = TypeParam::test(N, Y, incX);
    EXPECT_EQ(0, res2);
}

TYPED_TEST(IaminComplexTest, empty) {
    using Complex = typename TypeParam::complex_type;

    Complex X[1] = {{1.0, 0.0}};

    int32_t res = TypeParam::test(0, X, 1);
    EXPECT_EQ(0, res);
}

}
