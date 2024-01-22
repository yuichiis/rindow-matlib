#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdbool.h>

using testing::ContainerEq;

namespace {
TEST(AddTest, AddNormal) {
    const int M = 2;
    const int N = 3;
    // float
    float sX[N] = {1, 2, 3};
    float sY[M][N] = {{1,10,100}, {-1,-10,-100}};
    rindow_matlib_s_add(
        RINDOW_MATLIB_NO_TRANS, // int32_t trans,
        M,      // int32_t m,
        N,      // int32_t n,
        2.0,    // float alpha,
        (float *)&sX,     // float *x,
        1,      // int32_t incX,
        (float *)&sY,     // float *a,
        N       // int32_t ldA
    );
    float sR1[N] = {1,2,3};
    EXPECT_THAT(sR1, ContainerEq(sX));
    float sR2[M][N] = {{3,14,106}, {1,-6,-94}};
    EXPECT_THAT(sR2, ContainerEq(sY));

    // double
    double dX[N] = {1, 2, 3};
    double dY[M][N] = {{1,10,100}, {-1,-10,-100}};
    rindow_matlib_d_add(
        RINDOW_MATLIB_NO_TRANS, // int32_t trans,
        M,      // int32_t m,
        N,      // int32_t n,
        2.0,    // float alpha,
        (double *)&dX,     // float *x,
        1,      // int32_t incX,
        (double *)&dY,     // float *a,
        N       // int32_t ldA
    );
    double dR1[N] = {1,2,3};
    EXPECT_THAT(dR1, ContainerEq(dX));
    double dR2[M][N] = {{3,14,106}, {1,-6,-94}};
    EXPECT_THAT(dR2, ContainerEq(dY));
}

TEST(AddTest, AddTrans) {
    const int M = 2;
    const int N = 3;
    // float
    float sX[N] = {1, 2, 3};
    float sY[N][M] = {{1,10}, {100,-1}, {-10,-100}};
    rindow_matlib_s_add(
        RINDOW_MATLIB_TRANS, // int32_t trans,
        N,      // int32_t m,
        M,      // int32_t n,
        1.0,    // float alpha,
        (float *)&sX,     // float *x,
        1,      // int32_t incX,
        (float *)&sY,     // float *a,
        M       // int32_t ldA
    );
    float sR1[N] = {1,2,3};
    EXPECT_THAT(sR1, ContainerEq(sX));
    float sR2[N][M] = {{2,11},{102,1},{-7,-97}};
    EXPECT_THAT(sR2, ContainerEq(sY));

    // double
    double dX[N] = {1, 2, 3};
    double dY[N][M] = {{1,10}, {100,-1}, {-10,-100}};
    rindow_matlib_d_add(
        RINDOW_MATLIB_TRANS, // int32_t trans,
        N,      // int32_t m,
        M,      // int32_t n,
        1.0,    // float alpha,
        (double *)&dX,     // float *x,
        1,      // int32_t incX,
        (double *)&dY,     // float *a,
        M       // int32_t ldA
    );
    double dR1[N] = {1,2,3};
    EXPECT_THAT(dR1, ContainerEq(dX));
    double dR2[N][M] = {{2,11},{102,1},{-7,-97}};
    EXPECT_THAT(dR2, ContainerEq(dY));
}
}
