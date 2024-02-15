#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <limits>

using testing::ContainerEq;

namespace {

template <typename T>
class IminTest : public ::testing::Test {
protected:
    virtual int32_t test_matlib_imin(int32_t n,float *x, int32_t incX, float floatMax)
    {
        return rindow_matlib_s_imin(n,x, incX, floatMax);
    }

    virtual int32_t test_matlib_imin(int32_t n,double *x, int32_t incX, double floatMax)
    {
        return rindow_matlib_d_imin(n,x, incX, floatMax);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(IminTest, TestTypes);

TYPED_TEST(IminTest, IminNormal) {
    const int32_t M = 1;
    const int32_t N = 6;
    const int32_t incX = 1;
    TypeParam X[N] = {-1, 2,-3,-4, 5,-6};
    TypeParam inf = std::numeric_limits<TypeParam>::infinity();

    int32_t res = this->test_matlib_imin(N, X, incX, (TypeParam)(inf));
    EXPECT_EQ(5, res);
}

}
