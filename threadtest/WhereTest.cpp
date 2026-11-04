#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <algorithm>
#include <vector>

#include "rindow/matlib.h"

using testing::ContainerEq;

namespace {

// Same values as defined in where.cpp
constexpr int32_t KIND_MATRIX = 0;
constexpr int32_t KIND_VECTOR = 1;
constexpr int32_t KIND_SCALAR = 2;

template <typename T>
class WhereTest : public ::testing::Test {
protected:
    virtual void test_matlib_where(
        int32_t trans,int32_t m,int32_t n,
        const uint8_t *cond, int32_t kindC,
        const float *x, int32_t kindX,
        const float *y, int32_t kindY,
        float *out
    ) {
        rindow_matlib_s_where(trans,m,n,cond,kindC,x,kindX,y,kindY,out);
    }
    virtual void test_matlib_where(
        int32_t trans,int32_t m,int32_t n,
        const uint8_t *cond, int32_t kindC,
        const double *x, int32_t kindX,
        const double *y, int32_t kindY,
        double *out
    ) {
        rindow_matlib_d_where(trans,m,n,cond,kindC,x,kindX,y,kindY,out);
    }

    // Compare with reference implementation for cases where cond is a vector and X, Y are matrices (for checking parallelization)
    void run_cond_vector_case(int32_t trans, int32_t m, int32_t n) {
        const int32_t len = (trans==RINDOW_MATLIB_NO_TRANS) ? n : m;
        std::vector<uint8_t> cond(len);
        std::vector<T> x(m*n), y(m*n), out(m*n, static_cast<T>(-999)), expected(m*n);
        for(int32_t k=0; k<len; k++) {
            cond[k] = static_cast<uint8_t>((k%3==0) ? 1 : 0);
        }
        for(int32_t k=0; k<m*n; k++) {
            x[k] = static_cast<T>(k+1);
            y[k] = static_cast<T>(-(k+1));
        }
        for(int32_t i=0; i<m; i++) {
            for(int32_t j=0; j<n; j++) {
                int32_t c = (trans==RINDOW_MATLIB_NO_TRANS) ? j : i;
                expected[i*n+j] = cond[c] ? x[i*n+j] : y[i*n+j];
            }
        }
        this->test_matlib_where(
            trans,m,n,
            cond.data(),KIND_VECTOR,
            x.data(),KIND_MATRIX,
            y.data(),KIND_MATRIX,
            out.data());
        EXPECT_THAT(out, ContainerEq(expected));
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(WhereTest, TestTypes);

TYPED_TEST(WhereTest, matrix_rowsLTcols) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 2;
    const int32_t N = 3;

    uint8_t COND[M*N] = {1,0,1, 0,1,0};
    TypeParam X[M*N] = {1,2,3, 4,5,6};
    TypeParam Y[M*N] = {-1,-2,-3, -4,-5,-6};
    TypeParam OUT[M*N];
    std::fill_n(OUT, M*N, (TypeParam)-999);
    this->test_matlib_where(
        trans,M,N,
        COND,KIND_MATRIX,
        X,KIND_MATRIX,
        Y,KIND_MATRIX,
        OUT);
    uint8_t RC[M*N] = {1,0,1, 0,1,0};
    EXPECT_THAT(RC, ContainerEq(COND));
    TypeParam RX[M*N] = {1,2,3, 4,5,6};
    EXPECT_THAT(RX, ContainerEq(X));
    TypeParam RY[M*N] = {-1,-2,-3, -4,-5,-6};
    EXPECT_THAT(RY, ContainerEq(Y));
    TypeParam R[M*N] = {1,-2,3, -4,5,-6};
    EXPECT_THAT(R, ContainerEq(OUT));
}

TYPED_TEST(WhereTest, matrix_rowsGTcols) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;

    uint8_t COND[M*N] = {1,0, 0,1, 1,1};
    TypeParam X[M*N] = {1,2, 3,4, 5,6};
    TypeParam Y[M*N] = {-1,-2, -3,-4, -5,-6};
    TypeParam OUT[M*N];
    std::fill_n(OUT, M*N, (TypeParam)-999);
    this->test_matlib_where(
        trans,M,N,
        COND,KIND_MATRIX,
        X,KIND_MATRIX,
        Y,KIND_MATRIX,
        OUT);
    TypeParam R[M*N] = {1,-2, -3,4, 5,6};
    EXPECT_THAT(R, ContainerEq(OUT));
}

TYPED_TEST(WhereTest, cond_vector_notrans) {
    // Apply cond(n) to each row
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;

    uint8_t COND[N] = {1,0};
    TypeParam X[M*N] = {1,2, 3,4, 5,6};
    TypeParam Y[M*N] = {-1,-2, -3,-4, -5,-6};
    TypeParam OUT[M*N];
    std::fill_n(OUT, M*N, (TypeParam)-999);
    this->test_matlib_where(
        trans,M,N,
        COND,KIND_VECTOR,
        X,KIND_MATRIX,
        Y,KIND_MATRIX,
        OUT);
    uint8_t RC[N] = {1,0};
    EXPECT_THAT(RC, ContainerEq(COND));
    TypeParam R[M*N] = {1,-2, 3,-4, 5,-6};
    EXPECT_THAT(R, ContainerEq(OUT));
}

TYPED_TEST(WhereTest, cond_vector_trans_rowsGTcols) {
    // Select row-wise using cond(m)
    const int32_t trans = RINDOW_MATLIB_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;

    uint8_t COND[M] = {1,0,1};
    TypeParam X[M*N] = {1,2, 3,4, 5,6};
    TypeParam Y[M*N] = {-1,-2, -3,-4, -5,-6};
    TypeParam OUT[M*N];
    std::fill_n(OUT, M*N, (TypeParam)-999);
    this->test_matlib_where(
        trans,M,N,
        COND,KIND_VECTOR,
        X,KIND_MATRIX,
        Y,KIND_MATRIX,
        OUT);
    uint8_t RC[M] = {1,0,1};
    EXPECT_THAT(RC, ContainerEq(COND));
    TypeParam R[M*N] = {1,2, -3,-4, 5,6};
    EXPECT_THAT(R, ContainerEq(OUT));
}

TYPED_TEST(WhereTest, cond_vector_trans_rowsLTcols) {
    const int32_t trans = RINDOW_MATLIB_TRANS;
    const int32_t M = 2;
    const int32_t N = 3;

    uint8_t COND[M] = {0,1};
    TypeParam X[M*N] = {1,2,3, 4,5,6};
    TypeParam Y[M*N] = {-1,-2,-3, -4,-5,-6};
    TypeParam OUT[M*N];
    std::fill_n(OUT, M*N, (TypeParam)-999);
    this->test_matlib_where(
        trans,M,N,
        COND,KIND_VECTOR,
        X,KIND_MATRIX,
        Y,KIND_MATRIX,
        OUT);
    TypeParam R[M*N] = {-1,-2,-3, 4,5,6};
    EXPECT_THAT(R, ContainerEq(OUT));
}

TYPED_TEST(WhereTest, scalar_x_and_y) {
    // where(cond, 7, 0)
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 2;
    const int32_t N = 3;

    uint8_t COND[M*N] = {1,0,1, 0,0,1};
    TypeParam X[1] = {7};
    TypeParam Y[1] = {0};
    TypeParam OUT[M*N];
    std::fill_n(OUT, M*N, (TypeParam)-999);
    this->test_matlib_where(
        trans,M,N,
        COND,KIND_MATRIX,
        X,KIND_SCALAR,
        Y,KIND_SCALAR,
        OUT);
    TypeParam RX[1] = {7};
    EXPECT_THAT(RX, ContainerEq(X));
    TypeParam RY[1] = {0};
    EXPECT_THAT(RY, ContainerEq(Y));
    TypeParam R[M*N] = {7,0,7, 0,0,7};
    EXPECT_THAT(R, ContainerEq(OUT));
}

TYPED_TEST(WhereTest, x_vector_notrans_y_scalar) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;

    uint8_t COND[M*N] = {1,0, 1,1, 0,1};
    TypeParam X[N] = {10,20};
    TypeParam Y[1] = {0};
    TypeParam OUT[M*N];
    std::fill_n(OUT, M*N, (TypeParam)-999);
    this->test_matlib_where(
        trans,M,N,
        COND,KIND_MATRIX,
        X,KIND_VECTOR,
        Y,KIND_SCALAR,
        OUT);
    TypeParam R[M*N] = {10,0, 10,20, 0,20};
    EXPECT_THAT(R, ContainerEq(OUT));
}

TYPED_TEST(WhereTest, x_vector_trans_y_vector_trans) {
    // Apply X(m) and Y(m) row-wise, cond is a matrix
    const int32_t trans = RINDOW_MATLIB_TRANS;
    const int32_t M = 3;
    const int32_t N = 2;

    uint8_t COND[M*N] = {1,0, 1,0, 0,1};
    TypeParam X[M] = {10,20,30};
    TypeParam Y[M] = {-1,-2,-3};
    TypeParam OUT[M*N];
    std::fill_n(OUT, M*N, (TypeParam)-999);
    this->test_matlib_where(
        trans,M,N,
        COND,KIND_MATRIX,
        X,KIND_VECTOR,
        Y,KIND_VECTOR,
        OUT);
    TypeParam R[M*N] = {10,-1, 20,-2, -3,30};
    EXPECT_THAT(R, ContainerEq(OUT));
}

TYPED_TEST(WhereTest, all_vector_notrans) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 2;
    const int32_t N = 3;

    uint8_t COND[N] = {1,0,1};
    TypeParam X[N] = {1,2,3};
    TypeParam Y[N] = {-1,-2,-3};
    TypeParam OUT[M*N];
    std::fill_n(OUT, M*N, (TypeParam)-999);
    this->test_matlib_where(
        trans,M,N,
        COND,KIND_VECTOR,
        X,KIND_VECTOR,
        Y,KIND_VECTOR,
        OUT);
    TypeParam R[M*N] = {1,-2,3, 1,-2,3};
    EXPECT_THAT(R, ContainerEq(OUT));
}

TYPED_TEST(WhereTest, nonzero_cond_is_true) {
    // Treat all non-zero values as true
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;
    const int32_t M = 1;
    const int32_t N = 4;

    uint8_t COND[M*N] = {0,1,2,255};
    TypeParam X[M*N] = {1,2,3,4};
    TypeParam Y[M*N] = {-1,-2,-3,-4};
    TypeParam OUT[M*N];
    std::fill_n(OUT, M*N, (TypeParam)-999);
    this->test_matlib_where(
        trans,M,N,
        COND,KIND_MATRIX,
        X,KIND_MATRIX,
        Y,KIND_MATRIX,
        OUT);
    TypeParam R[M*N] = {-1,2,3,4};
    EXPECT_THAT(R, ContainerEq(OUT));
}

TYPED_TEST(WhereTest, parallel_notrans_rowsGTcols) {
    this->run_cond_vector_case(RINDOW_MATLIB_NO_TRANS, 1000, 7);
}

TYPED_TEST(WhereTest, parallel_notrans_rowsLTcols) {
    this->run_cond_vector_case(RINDOW_MATLIB_NO_TRANS, 7, 1000);
}

TYPED_TEST(WhereTest, parallel_trans_rowsGTcols) {
    this->run_cond_vector_case(RINDOW_MATLIB_TRANS, 1000, 7);
}

TYPED_TEST(WhereTest, parallel_trans_rowsLTcols) {
    this->run_cond_vector_case(RINDOW_MATLIB_TRANS, 7, 1000);
}

TYPED_TEST(WhereTest, empty_size_does_nothing) {
    const int32_t trans = RINDOW_MATLIB_NO_TRANS;

    uint8_t COND[1] = {1};
    TypeParam X[1] = {1};
    TypeParam Y[1] = {2};
    TypeParam OUT[1] = {-999};
    this->test_matlib_where(
        trans,0,3,
        COND,KIND_SCALAR,
        X,KIND_SCALAR,
        Y,KIND_SCALAR,
        OUT);
    this->test_matlib_where(
        trans,3,0,
        COND,KIND_SCALAR,
        X,KIND_SCALAR,
        Y,KIND_SCALAR,
        OUT);
    TypeParam R[1] = {-999};
    EXPECT_THAT(R, ContainerEq(OUT));
}

}