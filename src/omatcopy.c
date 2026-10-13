#include "rindow/matlib.h"
#include <stdint.h>
#include "common.h"

/*
 * B = alpha * op(A)
 *   op(A): NO_TRANS / TRANS / CONJ_NO_TRANS / CONJ_TRANS
 *   m, n : Number of rows and columns of A (for Trans cases, B is n x m)
 */

/* ColMajor (m x n) is equivalent to viewing the same memory as RowMajor (n x m).
 * Since transposition and conjugation are element-wise operations, this conversion does not change the result. */
static int rindow_matlib_omatcopy_normalize(
    int32_t order,
    int32_t trans,
    int32_t m,
    int32_t n,
    int32_t ldA,
    int32_t ldB,
    int32_t *rows,
    int32_t *cols,
    int32_t *doTrans,
    int32_t *doConj
)
{
    if(m <= 0 || n <= 0) {
        return -1;
    }

    if(order == RINDOW_MATLIB_ROW_MAJOR) {
        *rows = m;
        *cols = n;
    } else if(order == RINDOW_MATLIB_COL_MAJOR) {
        *rows = n;
        *cols = m;
    } else {
        return -1;
    }

    switch(trans) {
        case RINDOW_MATLIB_NO_TRANS:
            *doTrans = 0; *doConj = 0; break;
        case RINDOW_MATLIB_TRANS:
            *doTrans = 1; *doConj = 0; break;
        case RINDOW_MATLIB_CONJ_NO_TRANS:
            *doTrans = 0; *doConj = 1; break;
        case RINDOW_MATLIB_CONJ_TRANS:
            *doTrans = 1; *doConj = 1; break;
        default:
            return -1;
    }

    if(ldA < *cols) {
        return -1;
    }
    if(ldB < (*doTrans ? *rows : *cols)) {
        return -1;
    }
    return 0;
}

/* ---------------- Real numbers (s, d) ---------------- */

#define RINDOW_MATLIB_OMATCOPY_NOTRANS_TEMPLATE(a,b,i,j,ldA,ldB,alpha) \
    b[(int64_t)(i)*(ldB)+(j)] = (alpha) * a[(int64_t)(i)*(ldA)+(j)];

#define RINDOW_MATLIB_OMATCOPY_TRANS_TEMPLATE(a,b,i,j,ldA,ldB,alpha) \
    b[(int64_t)(j)*(ldB)+(i)] = (alpha) * a[(int64_t)(i)*(ldA)+(j)];


void rindow_matlib_s_omatcopy(
    int32_t order, int32_t trans, int32_t m, int32_t n,
    float alpha, const float *a, int32_t ldA, float *b, int32_t ldB)
{
    int32_t rows, cols, doTrans, doConj;
    int32_t i, j;

    if(rindow_matlib_omatcopy_normalize(
        order, trans, m, n, ldA, ldB, &rows, &cols, &doTrans, &doConj) != 0) {
        return;
    }

    if(!doTrans) {
        #pragma omp parallel for private(j)
        for(i=0;i<rows;i++) {
            for(j=0;j<cols;j++) {
                RINDOW_MATLIB_OMATCOPY_NOTRANS_TEMPLATE(a,b,i,j,ldA,ldB,alpha)
            }
        }
    } else {
        #pragma omp parallel for private(j)
        for(i=0;i<rows;i++) {
            for(j=0;j<cols;j++) {
                RINDOW_MATLIB_OMATCOPY_TRANS_TEMPLATE(a,b,i,j,ldA,ldB,alpha)
            }
        }
    }
}

void rindow_matlib_d_omatcopy(
    int32_t order, int32_t trans, int32_t m, int32_t n,
    double alpha, const double *a, int32_t ldA, double *b, int32_t ldB)
{
    int32_t rows, cols, doTrans, doConj;
    int32_t i, j;

    if(rindow_matlib_omatcopy_normalize(
        order, trans, m, n, ldA, ldB, &rows, &cols, &doTrans, &doConj) != 0) {
        return;
    }

    if(!doTrans) {
        #pragma omp parallel for private(j)
        for(i=0;i<rows;i++) {
            for(j=0;j<cols;j++) {
                RINDOW_MATLIB_OMATCOPY_NOTRANS_TEMPLATE(a,b,i,j,ldA,ldB,alpha)
            }
        }
    } else {
        #pragma omp parallel for private(j)
        for(i=0;i<rows;i++) {
            for(j=0;j<cols;j++) {
                RINDOW_MATLIB_OMATCOPY_TRANS_TEMPLATE(a,b,i,j,ldA,ldB,alpha)
            }
        }
    }
}

/* ---------------- Complex numbers (c, z) ---------------- */

/* dst = alpha * (conj ? conj(src) : src)
 * sign is -1 for conjugation, +1 otherwise */
#define RINDOW_MATLIB_OMATCOPY_COMPLEX_ELEMENT(dst,src,alphaR,alphaI,sign) \
    { \
        xr = (src).real; \
        xi = (sign) * (src).imag; \
        (dst).real = (alphaR) * xr - (alphaI) * xi; \
        (dst).imag = (alphaR) * xi + (alphaI) * xr; \
    }

#define RINDOW_MATLIB_OMATCOPY_COMPLEX_NOTRANS_TEMPLATE(a,b,i,j,ldA,ldB,alphaR,alphaI,sign) \
    RINDOW_MATLIB_OMATCOPY_COMPLEX_ELEMENT( \
        b[(int64_t)(i)*(ldB)+(j)], a[(int64_t)(i)*(ldA)+(j)], alphaR, alphaI, sign)

#define RINDOW_MATLIB_OMATCOPY_COMPLEX_TRANS_TEMPLATE(a,b,i,j,ldA,ldB,alphaR,alphaI,sign) \
    RINDOW_MATLIB_OMATCOPY_COMPLEX_ELEMENT( \
        b[(int64_t)(j)*(ldB)+(i)], a[(int64_t)(i)*(ldA)+(j)], alphaR, alphaI, sign)


void rindow_matlib_c_omatcopy(
    int32_t order, int32_t trans, int32_t m, int32_t n,
    const void *pAlpha, const void *pA, int32_t ldA,
    void *pB, int32_t ldB)
{
    const matlib_complex_float *alpha = pAlpha;
    const matlib_complex_float *a = pA;
    matlib_complex_float *b = pB;
    int32_t rows, cols, doTrans, doConj;
    int32_t i, j;
    float alphaR, alphaI, sign, xr, xi;

    if(alpha == NULL) {
        return;
    }
    if(rindow_matlib_omatcopy_normalize(
        order, trans, m, n, ldA, ldB, &rows, &cols, &doTrans, &doConj) != 0) {
        return;
    }

    /* Extract alpha as values before entering the parallel region */
    alphaR = alpha->real;
    alphaI = alpha->imag;
    sign = doConj ? -1.0f : 1.0f;

    if(!doTrans) {
        #pragma omp parallel for private(j, xr, xi)
        for(i=0;i<rows;i++) {
            for(j=0;j<cols;j++) {
                RINDOW_MATLIB_OMATCOPY_COMPLEX_NOTRANS_TEMPLATE(a,b,i,j,ldA,ldB,alphaR,alphaI,sign)
            }
        }
    } else {
        #pragma omp parallel for private(j, xr, xi)
        for(i=0;i<rows;i++) {
            for(j=0;j<cols;j++) {
                RINDOW_MATLIB_OMATCOPY_COMPLEX_TRANS_TEMPLATE(a,b,i,j,ldA,ldB,alphaR,alphaI,sign)
            }
        }
    }
}

void rindow_matlib_z_omatcopy(
    int32_t order, int32_t trans, int32_t m, int32_t n,
    const void *pAlpha, const void *pA, int32_t ldA,
    void *pB, int32_t ldB)
{
    const matlib_complex_double *alpha = pAlpha;
    const matlib_complex_double *a = pA;
    matlib_complex_double *b = pB;
    int32_t rows, cols, doTrans, doConj;
    int32_t i, j;
    double alphaR, alphaI, sign, xr, xi;

    if(alpha == NULL) {
        return;
    }
    if(rindow_matlib_omatcopy_normalize(
        order, trans, m, n, ldA, ldB, &rows, &cols, &doTrans, &doConj) != 0) {
        return;
    }

    alphaR = alpha->real;
    alphaI = alpha->imag;
    sign = doConj ? -1.0 : 1.0;

    if(!doTrans) {
        #pragma omp parallel for private(j, xr, xi)
        for(i=0;i<rows;i++) {
            for(j=0;j<cols;j++) {
                RINDOW_MATLIB_OMATCOPY_COMPLEX_NOTRANS_TEMPLATE(a,b,i,j,ldA,ldB,alphaR,alphaI,sign)
            }
        }
    } else {
        #pragma omp parallel for private(j, xr, xi)
        for(i=0;i<rows;i++) {
            for(j=0;j<cols;j++) {
                RINDOW_MATLIB_OMATCOPY_COMPLEX_TRANS_TEMPLATE(a,b,i,j,ldA,ldB,alphaR,alphaI,sign)
            }
        }
    }
}
