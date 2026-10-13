#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

// ---------------------------------------------------------------
// Real numbers (s, d)
// ---------------------------------------------------------------
template <typename T>
class Omatcopy
{
private:
    // Parallelize for each row i of A.
    // Even in the case of Trans, row i is written to column i of B, 
    // so the write destination does not conflict.
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t rows,
        int32_t cols,
        T alpha,
        const T *a,
        int32_t ldA,
        T *b,
        int32_t ldB,
        bool trans
    )
    {
        if(!trans) {
            for(int32_t i = cell.begin; i < cell.end; i++) {
                const T *srcRow = a + (int64_t)i * ldA;
                T *dstRow = b + (int64_t)i * ldB;
                for(int32_t j = 0; j < cols; j++) {
                    dstRow[j] = alpha * srcRow[j];
                }
            }
        } else {
            for(int32_t i = cell.begin; i < cell.end; i++) {
                const T *srcRow = a + (int64_t)i * ldA;
                for(int32_t j = 0; j < cols; j++) {
                    b[(int64_t)j * ldB + i] = alpha * srcRow[j];
                }
            }
        }
    }

public:
    static void execute(
        int32_t order,
        int32_t trans,
        int32_t m,
        int32_t n,
        T alpha,
        const T *a,
        int32_t ldA,
        T *b,
        int32_t ldB
    )
    {
        if(m <= 0 || n <= 0) {
            return;
        }

        // ColMajor is equivalent to viewing the same memory as RowMajor (n x m)
        int32_t rows = m;
        int32_t cols = n;
        if(order == RINDOW_MATLIB_COL_MAJOR) {
            rows = n;
            cols = m;
        } else if(order != RINDOW_MATLIB_ROW_MAJOR) {
            return;
        }

        bool doTrans;
        if(trans == RINDOW_MATLIB_NO_TRANS || trans == RINDOW_MATLIB_CONJ_NO_TRANS) {
            doTrans = false;  // In the case of real numbers, the conjugate does nothing.
        } else if(trans == RINDOW_MATLIB_TRANS || trans == RINDOW_MATLIB_CONJ_TRANS) {
            doTrans = true;
        } else {
            return;
        }

        if(ldA < cols) {
            return;
        }
        if(ldB < (doTrans ? rows : cols)) {
            return;
        }

        ParallelOperation::execute(rows, kernel, rows, cols, alpha, a, ldA, b, ldB, doTrans);
    }
};

// ---------------------------------------------------------------
// Complex numbers (c, z)   T: float/double, C: rindow_complex_float/double
// ---------------------------------------------------------------
template <typename T, typename C>
class OmatcopyComplex
{
private:
    static inline void scaleStore(
        C *dst, const C *src, T alphaR, T alphaI, bool conj
    )
    {
        T xr = src->real;
        T xi = conj ? -src->imag : src->imag;
        dst->real = alphaR * xr - alphaI * xi;
        dst->imag = alphaR * xi + alphaI * xr;
    }

    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t rows,
        int32_t cols,
        T alphaR,
        T alphaI,
        const C *a,
        int32_t ldA,
        C *b,
        int32_t ldB,
        bool trans,
        bool conj
    )
    {
        if(!trans) {
            for(int32_t i = cell.begin; i < cell.end; i++) {
                const C *srcRow = a + (int64_t)i * ldA;
                C *dstRow = b + (int64_t)i * ldB;
                for(int32_t j = 0; j < cols; j++) {
                    scaleStore(&dstRow[j], &srcRow[j], alphaR, alphaI, conj);
                }
            }
        } else {
            for(int32_t i = cell.begin; i < cell.end; i++) {
                const C *srcRow = a + (int64_t)i * ldA;
                for(int32_t j = 0; j < cols; j++) {
                    scaleStore(&b[(int64_t)j * ldB + i], &srcRow[j], alphaR, alphaI, conj);
                }
            }
        }
    }

public:
    static void execute(
        int32_t order,
        int32_t trans,
        int32_t m,
        int32_t n,
        const C *alpha,   // pointer argument
        const C *a,
        int32_t ldA,
        C *b,
        int32_t ldB
    )
    {
        if(m <= 0 || n <= 0 || alpha == nullptr) {
            return;
        }

        int32_t rows = m;
        int32_t cols = n;
        if(order == RINDOW_MATLIB_COL_MAJOR) {
            rows = n;
            cols = m;
        } else if(order != RINDOW_MATLIB_ROW_MAJOR) {
            return;
        }

        bool doTrans;
        bool doConj;
        switch(trans) {
            case RINDOW_MATLIB_NO_TRANS:      doTrans = false; doConj = false; break;
            case RINDOW_MATLIB_TRANS:         doTrans = true;  doConj = false; break;
            case RINDOW_MATLIB_CONJ_NO_TRANS: doTrans = false; doConj = true;  break;
            case RINDOW_MATLIB_CONJ_TRANS:    doTrans = true;  doConj = true;  break;
            default: return;
        }

        if(ldA < cols) {
            return;
        }
        if(ldB < (doTrans ? rows : cols)) {
            return;
        }

        //  alpha to value before parallel execution
        T alphaR = alpha->real;
        T alphaI = alpha->imag;

        ParallelOperation::execute(
            rows, kernel, rows, cols, alphaR, alphaI, a, ldA, b, ldB, doTrans, doConj);
    }
};

}

extern "C" {

void rindow_matlib_s_omatcopy(
    int32_t order, int32_t trans, int32_t m, int32_t n,
    float alpha, const float *a, int32_t ldA, float *b, int32_t ldB)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Omatcopy<float>::execute(order, trans, m, n, alpha, a, ldA, b, ldB);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_omatcopy(
    int32_t order, int32_t trans, int32_t m, int32_t n,
    double alpha, const double *a, int32_t ldA, double *b, int32_t ldB)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Omatcopy<double>::execute(order, trans, m, n, alpha, a, ldA, b, ldB);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_c_omatcopy(
    int32_t order, int32_t trans, int32_t m, int32_t n,
    const void *alpha, const void *a, int32_t ldA,
    void *b, int32_t ldB)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    OmatcopyComplex<float, matlib_complex_float>::execute(
        order, trans, m, n, (matlib_complex_float*)alpha, (matlib_complex_float*)a, ldA, (matlib_complex_float*)b, ldB);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_z_omatcopy(
    int32_t order, int32_t trans, int32_t m, int32_t n,
    const void *alpha, const void *a, int32_t ldA,
    void *b, int32_t ldB)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    OmatcopyComplex<double, matlib_complex_double>::execute(
        order, trans, m, n, (matlib_complex_double*)alpha, (matlib_complex_double*)a, ldA, (matlib_complex_double*)b, ldB);
    RINDOW_END_CLEAR_EXCEPTION;
} 

}