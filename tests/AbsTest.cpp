#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class AbsTest : public ::testing::Test {
protected:
    virtual void test_matlib_abs(int32_t n, float *x, int32_t incX)
    {
        rindow_matlib_s_abs(n, x, incX);
    }

    virtual void test_matlib_abs(int32_t n, double *x, int32_t incX)
    {
        rindow_matlib_d_abs(n, x, incX);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(AbsTest, TestTypes);

TYPED_TEST(AbsTest, normal) {
    const int32_t M = 1;
    const int32_t N = 4;
    const int32_t incX = 1;
    TypeParam X[N] = {1.0, -2.0, 4.0, -9.0};

    this->test_matlib_abs(N, X, incX);

    TypeParam R1[N] = {1.0,2.0,4.0,9.0};
    EXPECT_TRUE(Utils::isclose(N,R1,X));
}

}
