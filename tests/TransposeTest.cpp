#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdbool.h>

using testing::ContainerEq;

namespace {
TEST(TransposeTest, Float1DNormal) {
    const int N = 3;
    // float
    float a[N] = {1, 2, 3};
    float b[N] = {0, 0, 0};

    const int32_t ndim = 1;
    int32_t shape[ndim] = { N };
    int32_t perm[ndim]  = { 0 };
    int32_t rc;

    rc = rindow_matlib_s_transpose(
        ndim,   // int32_t ndim,
        shape,  // int32_t shape[ndim],
        perm,   // int32_t perm[ndim],
        a,      // float a[N]
        b       // float b[N]
    );
    ASSERT_EQ(0, rc);
    float sR1[N] = {1,2,3};
    EXPECT_THAT(sR1, ContainerEq(a));
    float sR2[N] = {1,2,3};
    EXPECT_THAT(sR2, ContainerEq(b));
}

TEST(TransposeTest, Double1DNormal) {
    const int N = 3;
    // float
    double a[N] = {1, 2, 3};
    double b[N] = {0, 0, 0};

    const int32_t ndim = 1;
    int32_t shape[ndim] = { N };
    int32_t perm[ndim]  = { 0 };
    int32_t rc;

    rc = rindow_matlib_d_transpose(
        ndim,   // int32_t ndim,
        shape,  // int32_t shape[ndim],
        perm,   // int32_t perm[ndim],
        a,      // double a[N]
        b       // double b[N]
    );
    ASSERT_EQ(0, rc);
    double sR1[N] = {1,2,3};
    EXPECT_THAT(sR1, ContainerEq(a));
    double sR2[N] = {1,2,3};
    EXPECT_THAT(sR2, ContainerEq(b));
}

TEST(TransposeTest, Integer1DNormal) {
    const int N = 3;
    // float
    int32_t a[N] = {1, 2, 3};
    int32_t b[N] = {0, 0, 0};

    int32_t dtype = rindow_matlib_dtype_int32;
    const int32_t ndim = 1;
    int32_t shape[ndim] = { N };
    int32_t perm[ndim]  = { 0 };
    int32_t rc;

    rc = rindow_matlib_i_transpose(
        dtype,  // int32_t dtype,
        ndim,   // int32_t ndim,
        shape,  // int32_t shape[ndim],
        perm,   // int32_t perm[ndim],
        a,      // void* a,
        b       // void* b
    );
    ASSERT_EQ(0, rc);
    int32_t sR1[N] = {1,2,3};
    EXPECT_THAT(sR1, ContainerEq(a));
    int32_t sR2[N] = {1,2,3};
    EXPECT_THAT(sR2, ContainerEq(b));
}
TEST(TransposeTest, Float2DNormal) {
    const int M = 2;
    const int N = 3;
    // float
    float a[M*N] = {
        1, 2, 3,
        4, 5, 6
    };
    float b[M*N] = {
        0, 0, 0,
        0, 0, 0
    };

    const int32_t ndim = 2;
    int32_t shape[ndim] = { M, N };
    int32_t perm[ndim]  = { 1, 0 };
    int32_t rc;

    rc = rindow_matlib_s_transpose(
        ndim,   // int32_t ndim,
        shape,  // int32_t shape[ndim],}
        perm,   // int32_t perm[ndim],
        a,      // float a[N]
        b       // float b[N]
    );
    ASSERT_EQ(0, rc);
    float sR1[M*N] = {
        1, 2, 3,
        4, 5, 6
    };
    EXPECT_THAT(sR1, ContainerEq(a));
    float sR2[N*M] = {
        1, 4,  
        2, 5,
        3, 6
    };
    EXPECT_THAT(sR2, ContainerEq(b));
}
TEST(TransposeTest, Double2DNormal) {
    const int M = 2;
    const int N = 3;
    // float
    double a[M*N] = {
        1, 2, 3,
        4, 5, 6
    };
    double b[M*N] = {
        0, 0, 0,
        0, 0, 0
    };

    const int32_t ndim = 2;
    int32_t shape[ndim] = { M, N };
    int32_t perm[ndim]  = { 1, 0 };
    int32_t rc;

    rc = rindow_matlib_d_transpose(
        ndim,   // int32_t ndim,
        shape,  // int32_t shape[ndim],}
        perm,   // int32_t perm[ndim],
        a,      // double a[N]
        b       // double b[N]
    );
    ASSERT_EQ(0, rc);
    double sR1[M*N] = {
        1, 2, 3,
        4, 5, 6
    };
    EXPECT_THAT(sR1, ContainerEq(a));
    double sR2[N*M] = {
        1, 4,  
        2, 5,
        3, 6
    };
    EXPECT_THAT(sR2, ContainerEq(b));
}
TEST(TransposeTest, Integer2DNormal) {
    const int M = 2;
    const int N = 3;
    // float
    int32_t a[M*N] = {
        1, 2, 3,
        4, 5, 6
    };
    int32_t b[M*N] = {
        0, 0, 0,
        0, 0, 0
    };

    int32_t dtype = rindow_matlib_dtype_int32;
    const int32_t ndim = 2;
    int32_t shape[ndim] = { M, N };
    int32_t perm[ndim]  = { 1, 0 };
    int32_t rc;

    rc = rindow_matlib_i_transpose(
        dtype,  // int32_t dtype, 
        ndim,   // int32_t ndim,
        shape,  // int32_t shape[ndim],
        perm,   // int32_t perm[ndim],
        a,      // void *a
        b       // void *b
    );
    ASSERT_EQ(0, rc);
    int32_t sR1[M*N] = {
        1, 2, 3,
        4, 5, 6
    };
    EXPECT_THAT(sR1, ContainerEq(a));
    int32_t sR2[N*M] = {
        1, 4,  
        2, 5,
        3, 6
    };
    EXPECT_THAT(sR2, ContainerEq(b));
}
TEST(TransposeTest, Float3DNormal) {
    const int M = 2;
    const int N = 3;
    const int K = 4;
    const int SIZE = M*N*K;
    // float
    float a[SIZE];
    for(int i=0; i<SIZE; i++) { a[i] = (float)i; }
    float b[SIZE];
    for(int i=0; i<SIZE; i++) { b[i] = 0; }

    const int32_t ndim = 3;
    int32_t shape[ndim] = { M, N, K };
    int32_t perm[ndim]  = { 2, 1, 0 };
    int32_t rc;

    rc = rindow_matlib_s_transpose(
        ndim,   // int32_t ndim,
        shape,  // int32_t shape[ndim],}
        perm,   // int32_t perm[ndim],
        a,      // float a[N]
        b       // float b[N]
    );
    ASSERT_EQ(0, rc);
    float sR1[SIZE];
    for(int i=0; i<SIZE; i++) { sR1[i] = (float)i; }
    EXPECT_THAT(sR1, ContainerEq(a));
    float sR2[SIZE];
    for(int h=0;h<K;h++) {
        for(int i=0;i<N;i++) {
            for(int j=0;j<M;j++) {
                sR2[h*N*M+i*M+j] = (float)(j*N*K+i*K+h);
            }
        }
    }
    EXPECT_THAT(sR2, ContainerEq(b));
}
TEST(TransposeTest, Double3DNormal) {
    const int M = 2;
    const int N = 3;
    const int K = 4;
    const int SIZE = M*N*K;

    double a[SIZE];
    for(int i=0; i<SIZE; i++) { a[i] = (double)i; }
    double b[SIZE];
    for(int i=0; i<SIZE; i++) { b[i] = 0; }

    const int32_t ndim = 3;
    int32_t shape[ndim] = { M, N, K };
    int32_t perm[ndim]  = { 2, 1, 0 };
    int32_t rc;

    rc = rindow_matlib_d_transpose(
        ndim,   // int32_t ndim,
        shape,  // int32_t shape[ndim],}
        perm,   // int32_t perm[ndim],
        a,      // double a[N]
        b       // double b[N]
    );
    ASSERT_EQ(0, rc);
    double sR1[SIZE];
    for(int i=0; i<SIZE; i++) { sR1[i] = (double)i; }
    EXPECT_THAT(sR1, ContainerEq(a));
    double sR2[SIZE];
    for(int h=0;h<K;h++) {
        for(int i=0;i<N;i++) {
            for(int j=0;j<M;j++) {
                sR2[h*N*M+i*M+j] = (double)(j*N*K+i*K+h);
            }
        }
    }
    EXPECT_THAT(sR2, ContainerEq(b));
}
TEST(TransposeTest, Integer3DNormal) {
    const int M = 2;
    const int N = 3;
    const int K = 4;
    const int SIZE = M*N*K;

    int32_t a[SIZE];
    for(int i=0; i<SIZE; i++) { a[i] = (int32_t)i; }
    int32_t b[SIZE];
    for(int i=0; i<SIZE; i++) { b[i] = 0; }

    int32_t dtype = rindow_matlib_dtype_int32;
    const int32_t ndim = 3;
    int32_t shape[ndim] = { M, N, K };
    int32_t perm[ndim]  = { 2, 1, 0 };
    int32_t rc;

    rc = rindow_matlib_i_transpose(
        dtype,  // int32_t dtype, 
        ndim,   // int32_t ndim,
        shape,  // int32_t shape[ndim],}
        perm,   // int32_t perm[ndim],
        a,      // void* a
        b       // void* b
    );
    ASSERT_EQ(0, rc);
    int32_t sR1[SIZE];
    for(int i=0; i<SIZE; i++) { sR1[i] = (int32_t)i; }
    EXPECT_THAT(sR1, ContainerEq(a));
    int32_t sR2[SIZE];
    for(int h=0;h<K;h++) {
        for(int i=0;i<N;i++) {
            for(int j=0;j<M;j++) {
                sR2[h*N*M+i*M+j] = (int32_t)(j*N*K+i*K+h);
            }
        }
    }
    EXPECT_THAT(sR2, ContainerEq(b));
}
TEST(TransposeTest, Float3D2Normal) {
    const int M = 2;
    const int N = 3;
    const int K = 4;
    const int SIZE = M*N*K;
    // float
    float a[SIZE];
    for(int i=0; i<SIZE; i++) { a[i] = (float)i; }
    float b[SIZE];
    for(int i=0; i<SIZE; i++) { b[i] = 0; }

    const int32_t ndim = 3;
    int32_t shape[ndim] = { M, N, K };
    int32_t perm[ndim]  = { 0, 2, 1 };
    int32_t rc;

    rc = rindow_matlib_s_transpose(
        ndim,   // int32_t ndim,
        shape,  // int32_t shape[ndim],}
        perm,   // int32_t perm[ndim],
        a,      // float a[N]
        b       // float b[N]
    );
    ASSERT_EQ(0, rc);
    float sR1[SIZE];
    for(int i=0; i<SIZE; i++) { sR1[i] = (float)i; }
    EXPECT_THAT(sR1, ContainerEq(a));
    float sR2[SIZE];
    for(int h=0;h<M;h++) {
        for(int i=0;i<K;i++) {
            for(int j=0;j<N;j++) {
                sR2[h*K*N+i*N+j] = (float)(h*K*N+j*K+i);
            }
        }
    }
    EXPECT_THAT(sR2, ContainerEq(b));
}
TEST(TransposeTest, Double3D2Normal) {
    const int M = 2;
    const int N = 3;
    const int K = 4;
    const int SIZE = M*N*K;

    double a[SIZE];
    for(int i=0; i<SIZE; i++) { a[i] = (double)i; }
    double b[SIZE];
    for(int i=0; i<SIZE; i++) { b[i] = 0; }

    const int32_t ndim = 3;
    int32_t shape[ndim] = { M, N, K };
    int32_t perm[ndim]  = { 0, 2, 1 };
    int32_t rc;

    rc = rindow_matlib_d_transpose(
        ndim,   // int32_t ndim,
        shape,  // int32_t shape[ndim],}
        perm,   // int32_t perm[ndim],
        a,      // double a[N]
        b       // double b[N]
    );
    ASSERT_EQ(0, rc);
    double sR1[SIZE];
    for(int i=0; i<SIZE; i++) { sR1[i] = (double)i; }
    EXPECT_THAT(sR1, ContainerEq(a));
    double sR2[SIZE];
    for(int h=0;h<M;h++) {
        for(int i=0;i<K;i++) {
            for(int j=0;j<N;j++) {
                sR2[h*K*N+i*N+j] = (double)(h*K*N+j*K+i);
            }
        }
    }
    EXPECT_THAT(sR2, ContainerEq(b));
}
TEST(TransposeTest, Integer3D2Normal) {
    const int M = 2;
    const int N = 3;
    const int K = 4;
    const int SIZE = M*N*K;

    int32_t a[SIZE];
    for(int i=0; i<SIZE; i++) { a[i] = (int32_t)i; }
    int32_t b[SIZE];
    for(int i=0; i<SIZE; i++) { b[i] = 0; }

    int32_t dtype = rindow_matlib_dtype_int32;
    const int32_t ndim = 3;
    int32_t shape[ndim] = { M, N, K };
    int32_t perm[ndim]  = { 0, 2, 1 };
    int32_t rc;

    rc = rindow_matlib_i_transpose(
        dtype,  // int32_t dtype, 
        ndim,   // int32_t ndim,
        shape,  // int32_t shape[ndim],}
        perm,   // int32_t perm[ndim],
        a,      // void* a
        b       // void* b
    );
    ASSERT_EQ(0, rc);
    int32_t sR1[SIZE];
    for(int i=0; i<SIZE; i++) { sR1[i] = (int32_t)i; }
    EXPECT_THAT(sR1, ContainerEq(a));
    int32_t sR2[SIZE];
    for(int h=0;h<M;h++) {
        for(int i=0;i<K;i++) {
            for(int j=0;j<N;j++) {
                sR2[h*K*N+i*N+j] = (int32_t)(h*K*N+j*K+i);
            }
        }
    }
    EXPECT_THAT(sR2, ContainerEq(b));
}
}