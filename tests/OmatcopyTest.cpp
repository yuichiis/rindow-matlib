#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <limits>
#include <vector>
#include "Utils.h"

using RindowTest::Utils;

namespace {

static const int32_t ALL_ORDERS[2] = {
    RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_COL_MAJOR
};
static const int32_t ALL_TRANS[4] = {
    RINDOW_MATLIB_NO_TRANS, RINDOW_MATLIB_TRANS,
    RINDOW_MATLIB_CONJ_NO_TRANS, RINDOW_MATLIB_CONJ_TRANS
};

static bool isTransOp(int32_t trans)
{
    return trans == RINDOW_MATLIB_TRANS || trans == RINDOW_MATLIB_CONJ_TRANS;
}
static bool isConjOp(int32_t trans)
{
    return trans == RINDOW_MATLIB_CONJ_NO_TRANS || trans == RINDOW_MATLIB_CONJ_TRANS;
}

// (Rows, cols) of output matrix B. m and n are rows and cols of A.
static void dstShape(int32_t trans, int32_t m, int32_t n, int32_t &br, int32_t &bc)
{
    br = isTransOp(trans) ? n : m;
    bc = isTransOp(trans) ? m : n;
}

// ===============================================================
// Real numbers (s, d)
// ===============================================================
template <typename T>
class OmatcopyTest : public ::testing::Test {
protected:
    virtual void test_matlib_omatcopy(
        int32_t order, int32_t trans, int32_t m, int32_t n,
        float alpha, const float *a, int32_t ldA, float *b, int32_t ldB)
    {
        rindow_matlib_s_omatcopy(order, trans, m, n, alpha, a, ldA, b, ldB);
    }

    virtual void test_matlib_omatcopy(
        int32_t order, int32_t trans, int32_t m, int32_t n,
        double alpha, const double *a, int32_t ldA, double *b, int32_t ldB)
    {
        rindow_matlib_d_omatcopy(order, trans, m, n, alpha, a, ldA, b, ldB);
    }

    // Naive reference implementation
    static void reference(
        int32_t order, int32_t trans, int32_t m, int32_t n,
        T alpha, const T *a, int32_t ldA, T *b, int32_t ldB)
    {
        bool row = (order == RINDOW_MATLIB_ROW_MAJOR);
        bool tr = isTransOp(trans);
        for(int32_t i = 0; i < m; i++) {
            for(int32_t j = 0; j < n; j++) {
                T v = row ? a[(int64_t)i * ldA + j] : a[i + (int64_t)j * ldA];
                int32_t r = tr ? j : i;
                int32_t c = tr ? i : j;
                T *dst = row ? &b[(int64_t)r * ldB + c] : &b[r + (int64_t)c * ldB];
                *dst = alpha * v;
            }
        }
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(OmatcopyTest, TestTypes);

TYPED_TEST(OmatcopyTest, RowMajorNoTrans) {
    const int32_t M = 2;
    const int32_t N = 3;
    TypeParam A[M*N] = {1, 2, 3, 4, 5, 6};
    TypeParam B[M*N] = {0, 0, 0, 0, 0, 0};

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_NO_TRANS, M, N, 2, A, N, B, N);

    TypeParam R[M*N] = {2, 4, 6, 8, 10, 12};
    EXPECT_TRUE(Utils::isclose(M*N, R, B));
}

TYPED_TEST(OmatcopyTest, RowMajorTrans) {
    const int32_t M = 2;
    const int32_t N = 3;
    TypeParam A[M*N] = {1, 2, 3, 4, 5, 6};
    TypeParam B[M*N] = {0, 0, 0, 0, 0, 0};   // 3x2

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_TRANS, M, N, 2, A, N, B, M);

    TypeParam R[M*N] = {2, 8, 4, 10, 6, 12};
    EXPECT_TRUE(Utils::isclose(M*N, R, B));
}

TYPED_TEST(OmatcopyTest, ColMajorNoTrans) {
    const int32_t M = 2;
    const int32_t N = 3;
    // [[1,2,3],[4,5,6]] in column-major
    TypeParam A[M*N] = {1, 4, 2, 5, 3, 6};
    TypeParam B[M*N] = {0, 0, 0, 0, 0, 0};

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_COL_MAJOR, RINDOW_MATLIB_NO_TRANS, M, N, 2, A, M, B, M);

    TypeParam R[M*N] = {2, 8, 4, 10, 6, 12};
    EXPECT_TRUE(Utils::isclose(M*N, R, B));
}

TYPED_TEST(OmatcopyTest, ColMajorTrans) {
    const int32_t M = 2;
    const int32_t N = 3;
    TypeParam A[M*N] = {1, 4, 2, 5, 3, 6};
    TypeParam B[M*N] = {0, 0, 0, 0, 0, 0};   // 3x2 column-major

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_COL_MAJOR, RINDOW_MATLIB_TRANS, M, N, 2, A, M, B, N);

    // B = 2*A^T = [[2,8],[4,10],[6,12]] in column-major
    TypeParam R[M*N] = {2, 4, 6, 8, 10, 12};
    EXPECT_TRUE(Utils::isclose(M*N, R, B));
}

TYPED_TEST(OmatcopyTest, ConjIsIgnoredForReal) {
    const int32_t M = 2;
    const int32_t N = 3;
    TypeParam A[M*N] = {1, 2, 3, 4, 5, 6};
    TypeParam B1[M*N] = {0, 0, 0, 0, 0, 0};
    TypeParam B2[M*N] = {0, 0, 0, 0, 0, 0};

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_CONJ_NO_TRANS, M, N, 2, A, N, B1, N);
    TypeParam R1[M*N] = {2, 4, 6, 8, 10, 12};
    EXPECT_TRUE(Utils::isclose(M*N, R1, B1));

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_CONJ_TRANS, M, N, 2, A, N, B2, M);
    TypeParam R2[M*N] = {2, 8, 4, 10, 6, 12};
    EXPECT_TRUE(Utils::isclose(M*N, R2, B2));
}

TYPED_TEST(OmatcopyTest, NegativeAndZeroAlpha) {
    const int32_t M = 2;
    const int32_t N = 3;
    TypeParam A[M*N] = {1, 2, 3, 4, 5, 6};
    TypeParam B[M*N] = {9, 9, 9, 9, 9, 9};

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_NO_TRANS, M, N, -1, A, N, B, N);
    TypeParam R1[M*N] = {-1, -2, -3, -4, -5, -6};
    EXPECT_TRUE(Utils::isclose(M*N, R1, B));

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_NO_TRANS, M, N, 0, A, N, B, N);
    TypeParam R2[M*N] = {0, 0, 0, 0, 0, 0};
    EXPECT_TRUE(Utils::isclose(M*N, R2, B));
}

TYPED_TEST(OmatcopyTest, RowMajorNoTransWithLeadingDimension) {
    const int32_t M = 2;
    const int32_t N = 3;
    const int32_t ldA = 4;
    const int32_t ldB = 5;
    TypeParam A[M*ldA] = {1, 2, 3, 99,
                          4, 5, 6, 99};
    TypeParam B[M*ldB] = {-1, -1, -1, -1, -1,
                          -1, -1, -1, -1, -1};

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_NO_TRANS, M, N, 2, A, ldA, B, ldB);

    // Padding area must not be overwritten
    TypeParam R[M*ldB] = {2, 4, 6, -1, -1,
                          8, 10, 12, -1, -1};
    EXPECT_TRUE(Utils::isclose(M*ldB, R, B));
}

TYPED_TEST(OmatcopyTest, RowMajorTransWithLeadingDimension) {
    const int32_t M = 2;
    const int32_t N = 3;
    const int32_t ldA = 4;
    const int32_t ldB = 3;
    TypeParam A[M*ldA] = {1, 2, 3, 99,
                          4, 5, 6, 99};
    TypeParam B[N*ldB] = {-1, -1, -1,
                          -1, -1, -1,
                          -1, -1, -1};

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_TRANS, M, N, 2, A, ldA, B, ldB);

    TypeParam R[N*ldB] = {2, 8, -1,
                          4, 10, -1,
                          6, 12, -1};
    EXPECT_TRUE(Utils::isclose(N*ldB, R, B));
}

TYPED_TEST(OmatcopyTest, ZeroSizeDoesNothing) {
    TypeParam A[1] = {1};
    TypeParam B[1] = {7};

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_NO_TRANS, 0, 1, 2, A, 1, B, 1);
    EXPECT_EQ((TypeParam)7, B[0]);

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_NO_TRANS, 1, 0, 2, A, 1, B, 1);
    EXPECT_EQ((TypeParam)7, B[0]);
}

TYPED_TEST(OmatcopyTest, InvalidArgumentsDoNothing) {
    const int32_t M = 2;
    const int32_t N = 3;
    TypeParam A[M*N] = {1, 2, 3, 4, 5, 6};
    TypeParam B[M*N] = {7, 7, 7, 7, 7, 7};
    TypeParam R[M*N] = {7, 7, 7, 7, 7, 7};

    // Invalid order
    this->test_matlib_omatcopy(999, RINDOW_MATLIB_NO_TRANS, M, N, 2, A, N, B, N);
    EXPECT_TRUE(Utils::isclose(M*N, R, B));
    // Invalid trans
    this->test_matlib_omatcopy(RINDOW_MATLIB_ROW_MAJOR, 999, M, N, 2, A, N, B, N);
    EXPECT_TRUE(Utils::isclose(M*N, R, B));
    // ldA is too small
    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_NO_TRANS, M, N, 2, A, N - 1, B, N);
    EXPECT_TRUE(Utils::isclose(M*N, R, B));
    // ldB is too small (Since Trans, B is N x M, requiring ldB >= M)
    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_TRANS, M, N, 2, A, N, B, M - 1);
    EXPECT_TRUE(Utils::isclose(M*N, R, B));
}

// Compare all combinations against reference with larger matrices, including parallel execution
TYPED_TEST(OmatcopyTest, LargeAllCombinations) {
    const int32_t M = 257;
    const int32_t N = 131;

    for(int32_t order : ALL_ORDERS) {
        for(int32_t trans : ALL_TRANS) {
            bool row = (order == RINDOW_MATLIB_ROW_MAJOR);
            int32_t ldA = (row ? N : M) + 3;
            int32_t aOuter = row ? M : N;
            std::vector<TypeParam> A((size_t)ldA * aOuter);
            for(size_t k = 0; k < A.size(); k++) {
                A[k] = (TypeParam)((int32_t)(k % 17) - 8);
            }

            int32_t br, bc;
            dstShape(trans, M, N, br, bc);
            int32_t ldB = (row ? bc : br) + 2;
            int32_t bOuter = row ? br : bc;
            std::vector<TypeParam> B((size_t)ldB * bOuter, (TypeParam)-100);
            std::vector<TypeParam> R((size_t)ldB * bOuter, (TypeParam)-100);

            TypeParam alpha = (TypeParam)1.5;
            this->test_matlib_omatcopy(
                order, trans, M, N, alpha, A.data(), ldA, B.data(), ldB);
            TestFixture::reference(
                order, trans, M, N, alpha, A.data(), ldA, R.data(), ldB);

            EXPECT_TRUE(Utils::isclose((int32_t)R.size(), R.data(), B.data()))
                << "order=" << order << " trans=" << trans;
        }
    }
}

// ===============================================================
// Complex numbers (c, z)
// ===============================================================
template <typename T> struct ComplexOf;
template <> struct ComplexOf<float>  { typedef matlib_complex_float  type; };
template <> struct ComplexOf<double> { typedef matlib_complex_double type; };

template <typename T>
class ComplexOmatcopyTest : public ::testing::Test {
protected:
    typedef typename ComplexOf<T>::type C;

    virtual void test_matlib_omatcopy(
        int32_t order, int32_t trans, int32_t m, int32_t n,
        const matlib_complex_float *alpha, const matlib_complex_float *a, int32_t ldA,
        matlib_complex_float *b, int32_t ldB)
    {
        rindow_matlib_c_omatcopy(order, trans, m, n, alpha, a, ldA, b, ldB);
    }

    virtual void test_matlib_omatcopy(
        int32_t order, int32_t trans, int32_t m, int32_t n,
        const matlib_complex_double *alpha, const matlib_complex_double *a, int32_t ldA,
        matlib_complex_double *b, int32_t ldB)
    {
        rindow_matlib_z_omatcopy(order, trans, m, n, alpha, a, ldA, b, ldB);
    }

    // Compare complex array as real array (alternating real, imag)
    static bool close(int32_t n, C *expected, C *actual)
    {
        return Utils::isclose(
            n * 2, reinterpret_cast<T *>(expected), reinterpret_cast<T *>(actual));
    }

    static void reference(
        int32_t order, int32_t trans, int32_t m, int32_t n,
        C alpha, const C *a, int32_t ldA, C *b, int32_t ldB)
    {
        bool row = (order == RINDOW_MATLIB_ROW_MAJOR);
        bool tr = isTransOp(trans);
        bool cj = isConjOp(trans);
        for(int32_t i = 0; i < m; i++) {
            for(int32_t j = 0; j < n; j++) {
                C v = row ? a[(int64_t)i * ldA + j] : a[i + (int64_t)j * ldA];
                T xr = v.real;
                T xi = cj ? -v.imag : v.imag;
                int32_t r = tr ? j : i;
                int32_t c = tr ? i : j;
                C *dst = row ? &b[(int64_t)r * ldB + c] : &b[r + (int64_t)c * ldB];
                dst->real = alpha.real * xr - alpha.imag * xi;
                dst->imag = alpha.real * xi + alpha.imag * xr;
            }
        }
    }
};
TYPED_TEST_SUITE(ComplexOmatcopyTest, TestTypes);

// A(2x3) = [[1+2i, 3+4i, 5+6i],[7+8i, 9+10i, 11+12i]], alpha = 2+1i
// alpha*x      : s0=(0,5)  s1=(2,11) s2=(4,17) s3=(6,23) s4=(8,29)  s5=(10,35)
// alpha*conj(x): c0=(4,-3) c1=(10,-5) c2=(16,-7) c3=(22,-9) c4=(28,-11) c5=(34,-13)

TYPED_TEST(ComplexOmatcopyTest, RowMajorNoTrans) {
    typedef typename TestFixture::C C;
    const int32_t M = 2;
    const int32_t N = 3;
    C A[M*N] = {{1,2},{3,4},{5,6},{7,8},{9,10},{11,12}};
    C B[M*N] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
    C alpha = {2, 1};

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_NO_TRANS, M, N, &alpha, A, N, B, N);

    C R[M*N] = {{0,5},{2,11},{4,17},{6,23},{8,29},{10,35}};
    EXPECT_TRUE(TestFixture::close(M*N, R, B));
}

TYPED_TEST(ComplexOmatcopyTest, RowMajorTrans) {
    typedef typename TestFixture::C C;
    const int32_t M = 2;
    const int32_t N = 3;
    C A[M*N] = {{1,2},{3,4},{5,6},{7,8},{9,10},{11,12}};
    C B[M*N] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
    C alpha = {2, 1};

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_TRANS, M, N, &alpha, A, N, B, M);

    // Transpose without conjugate: s0,s3, s1,s4, s2,s5
    C R[M*N] = {{0,5},{6,23},{2,11},{8,29},{4,17},{10,35}};
    EXPECT_TRUE(TestFixture::close(M*N, R, B));
}

TYPED_TEST(ComplexOmatcopyTest, RowMajorConjNoTrans) {
    typedef typename TestFixture::C C;
    const int32_t M = 2;
    const int32_t N = 3;
    C A[M*N] = {{1,2},{3,4},{5,6},{7,8},{9,10},{11,12}};
    C B[M*N] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
    C alpha = {2, 1};

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_CONJ_NO_TRANS, M, N, &alpha, A, N, B, N);

    C R[M*N] = {{4,-3},{10,-5},{16,-7},{22,-9},{28,-11},{34,-13}};
    EXPECT_TRUE(TestFixture::close(M*N, R, B));
}

TYPED_TEST(ComplexOmatcopyTest, RowMajorConjTrans) {
    typedef typename TestFixture::C C;
    const int32_t M = 2;
    const int32_t N = 3;
    C A[M*N] = {{1,2},{3,4},{5,6},{7,8},{9,10},{11,12}};
    C B[M*N] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
    C alpha = {2, 1};

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_CONJ_TRANS, M, N, &alpha, A, N, B, M);

    // c0,c3, c1,c4, c2,c5
    C R[M*N] = {{4,-3},{22,-9},{10,-5},{28,-11},{16,-7},{34,-13}};
    EXPECT_TRUE(TestFixture::close(M*N, R, B));
}

TYPED_TEST(ComplexOmatcopyTest, ColMajorTrans) {
    typedef typename TestFixture::C C;
    const int32_t M = 2;
    const int32_t N = 3;
    // Matrix A above in column-major
    C A[M*N] = {{1,2},{7,8},{3,4},{9,10},{5,6},{11,12}};
    C B[M*N] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};   // 3x2 column-major
    C alpha = {2, 1};

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_COL_MAJOR, RINDOW_MATLIB_TRANS, M, N, &alpha, A, M, B, N);

    // B = alpha*A^T = [[s0,s3],[s1,s4],[s2,s5]] in column-major: s0,s1,s2,s3,s4,s5
    C R[M*N] = {{0,5},{2,11},{4,17},{6,23},{8,29},{10,35}};
    EXPECT_TRUE(TestFixture::close(M*N, R, B));
}

TYPED_TEST(ComplexOmatcopyTest, ColMajorConjTrans) {
    typedef typename TestFixture::C C;
    const int32_t M = 2;
    const int32_t N = 3;
    C A[M*N] = {{1,2},{7,8},{3,4},{9,10},{5,6},{11,12}};
    C B[M*N] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
    C alpha = {2, 1};

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_COL_MAJOR, RINDOW_MATLIB_CONJ_TRANS, M, N, &alpha, A, M, B, N);

    C R[M*N] = {{4,-3},{10,-5},{16,-7},{22,-9},{28,-11},{34,-13}};
    EXPECT_TRUE(TestFixture::close(M*N, R, B));
}

TYPED_TEST(ComplexOmatcopyTest, RealAlphaAndImaginaryAlpha) {
    typedef typename TestFixture::C C;
    const int32_t M = 1;
    const int32_t N = 2;
    C A[M*N] = {{1,2},{3,4}};
    C B[M*N] = {{0,0},{0,0}};

    // alpha = i : i*(1+2i) = -2+1i, i*(3+4i) = -4+3i
    C alphaI = {0, 1};
    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_NO_TRANS, M, N, &alphaI, A, N, B, N);
    C R1[M*N] = {{-2,1},{-4,3}};
    EXPECT_TRUE(TestFixture::close(M*N, R1, B));

    // When alpha = 1, ConjNoTrans is conjugate copy
    C alphaOne = {1, 0};
    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_CONJ_NO_TRANS, M, N, &alphaOne, A, N, B, N);
    C R2[M*N] = {{1,-2},{3,-4}};
    EXPECT_TRUE(TestFixture::close(M*N, R2, B));
}

TYPED_TEST(ComplexOmatcopyTest, AlphaIsNotModified) {
    typedef typename TestFixture::C C;
    C A[2] = {{1,2},{3,4}};
    C B[2] = {{0,0},{0,0}};
    C alpha = {2, 1};

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_NO_TRANS, 1, 2, &alpha, A, 2, B, 2);

    EXPECT_EQ((TypeParam)2, alpha.real);
    EXPECT_EQ((TypeParam)1, alpha.imag);
}

TYPED_TEST(ComplexOmatcopyTest, RowMajorTransWithLeadingDimension) {
    typedef typename TestFixture::C C;
    const int32_t M = 2;
    const int32_t N = 3;
    const int32_t ldA = 4;
    const int32_t ldB = 3;
    C A[M*ldA] = {{1,2},{3,4},{5,6},{99,99},
                  {7,8},{9,10},{11,12},{99,99}};
    C B[N*ldB] = {{-1,-1},{-1,-1},{-1,-1},
                  {-1,-1},{-1,-1},{-1,-1},
                  {-1,-1},{-1,-1},{-1,-1}};
    C alpha = {2, 1};

    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_CONJ_TRANS, M, N, &alpha, A, ldA, B, ldB);

    C R[N*ldB] = {{4,-3},{22,-9},{-1,-1},
                  {10,-5},{28,-11},{-1,-1},
                  {16,-7},{34,-13},{-1,-1}};
    EXPECT_TRUE(TestFixture::close(N*ldB, R, B));
}

TYPED_TEST(ComplexOmatcopyTest, InvalidArgumentsDoNothing) {
    typedef typename TestFixture::C C;
    const int32_t M = 1;
    const int32_t N = 2;
    C A[M*N] = {{1,2},{3,4}};
    C B[M*N] = {{7,7},{7,7}};
    C R[M*N] = {{7,7},{7,7}};
    C alpha = {2, 1};

    // alpha is NULL
    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_NO_TRANS, M, N, nullptr, A, N, B, N);
    EXPECT_TRUE(TestFixture::close(M*N, R, B));
    // Invalid order
    this->test_matlib_omatcopy(999, RINDOW_MATLIB_NO_TRANS, M, N, &alpha, A, N, B, N);
    EXPECT_TRUE(TestFixture::close(M*N, R, B));
    // Invalid trans
    this->test_matlib_omatcopy(RINDOW_MATLIB_ROW_MAJOR, 999, M, N, &alpha, A, N, B, N);
    EXPECT_TRUE(TestFixture::close(M*N, R, B));
    // Size 0
    this->test_matlib_omatcopy(
        RINDOW_MATLIB_ROW_MAJOR, RINDOW_MATLIB_NO_TRANS, 0, N, &alpha, A, N, B, N);
    EXPECT_TRUE(TestFixture::close(M*N, R, B));
}

// Compare all combinations (2 order x 4 trans) against reference with larger matrices, including parallel execution
TYPED_TEST(ComplexOmatcopyTest, LargeAllCombinations) {
    typedef typename TestFixture::C C;
    const int32_t M = 257;
    const int32_t N = 131;

    for(int32_t order : ALL_ORDERS) {
        for(int32_t trans : ALL_TRANS) {
            bool row = (order == RINDOW_MATLIB_ROW_MAJOR);
            int32_t ldA = (row ? N : M) + 3;
            int32_t aOuter = row ? M : N;
            std::vector<C> A((size_t)ldA * aOuter);
            for(size_t k = 0; k < A.size(); k++) {
                A[k].real = (TypeParam)((int32_t)(k % 7) - 3);
                A[k].imag = (TypeParam)((int32_t)(k % 5) - 2);
            }

            int32_t br, bc;
            dstShape(trans, M, N, br, bc);
            int32_t ldB = (row ? bc : br) + 2;
            int32_t bOuter = row ? br : bc;
            C sentinel = {-100, -100};
            std::vector<C> B((size_t)ldB * bOuter, sentinel);
            std::vector<C> R((size_t)ldB * bOuter, sentinel);

            C alpha = {(TypeParam)1.5, (TypeParam)-0.5};
            this->test_matlib_omatcopy(
                order, trans, M, N, &alpha, A.data(), ldA, B.data(), ldB);
            TestFixture::reference(
                order, trans, M, N, alpha, A.data(), ldA, R.data(), ldB);

            EXPECT_TRUE(TestFixture::close((int32_t)R.size(), R.data(), B.data()))
                << "order=" << order << " trans=" << trans;
        }
    }
}

}