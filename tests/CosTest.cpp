#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdbool.h>
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {
TEST(CosTest, CosNormal) {
    const size_t M = 1;
    const size_t N = 4;
    const int incX = 1;

    // float
    auto sX = Utils::array<float>({1.0, 2.0, 4.0, 9.0});
    rindow_matlib_s_cos(
        N,
        sX.get(),
        incX
    );
    auto sR1 = Utils::array<float>({cosf(1),cosf(2),cosf(4),cosf(9)});
    EXPECT_TRUE(Utils::isclose(N,sR1.get(),sX.get()));

    // double
    auto dX = Utils::array<double>({1.0, 2.0, 4.0, 9.0});
    rindow_matlib_d_cos(
        N,
        dX.get(),
        incX
    );
    auto dR1 = Utils::array<double>({cos(1),cos(2),cos(4),cos(9)});
    EXPECT_TRUE(Utils::isclose(N,dR1.get(),dX.get()));
}

}
