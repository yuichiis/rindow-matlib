#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

/*
 * Operand kinds
 *   KIND_MATRIX : Same shape as (m,n). Same layout as out (ld = n)
 *   KIND_VECTOR : If trans==NO_TRANS, n elements with [j]; otherwise m elements with [i]
 *   KIND_SCALAR : 1 element. Always [0]
 */
constexpr int32_t KIND_MATRIX = 0;
constexpr int32_t KIND_VECTOR = 1;
constexpr int32_t KIND_SCALAR = 2;

template <typename T>
class Where
{
private:
    template <typename V>
    static inline V pick(
        const V *p, int32_t kind, int32_t trans,
        int32_t n, int32_t i, int32_t j)
    {
        switch(kind) {
            case KIND_MATRIX:
                return p[i * n + j];
            case KIND_VECTOR:
                return (trans == RINDOW_MATLIB_NO_TRANS) ? p[j] : p[i];
            default:
                return p[0];
        }
    }

    static void kernel(
        ParallelOperation::cellInfo cell,
        bool para_m,
        int32_t trans,
        int32_t m,
        int32_t n,
        const uint8_t *cond, int32_t kindC,
        const T *x, int32_t kindX,
        const T *y, int32_t kindY,
        T *out
    )
    {
        int32_t begin_i,end_i,begin_j,end_j;
        if(para_m) {
            begin_i=cell.begin;
            end_i=cell.end;
            begin_j=0;
            end_j=n;
        } else {
            begin_i=0;
            end_i=m;
            begin_j=cell.begin;
            end_j=cell.end;
        }
        for(int32_t i=begin_i; i<end_i; i++) {
            for(int32_t j=begin_j; j<end_j; j++) {
                uint8_t c = pick<uint8_t>(cond, kindC, trans, n, i, j);
                out[i*n+j] = (c != 0)
                    ? pick<T>(x, kindX, trans, n, i, j)
                    : pick<T>(y, kindY, trans, n, i, j);
            }
        }
    }

public:
    /*
     * out(m,n) := cond ? x : y
     *
     * It is assumed that the decomposition into m and n, and the consistency
     * of each operand's kind and number of elements, are already validated on
     * the PHP side.
     */
    static void execute(
        int32_t trans,
        int32_t m,
        int32_t n,
        const uint8_t *cond, int32_t kindC,
        const T *x, int32_t kindX,
        const T *y, int32_t kindY,
        T *out
    )
    {
        if(m <= 0 || n <= 0) {
            return;
        }
        int32_t parallel;
        bool para_m;
        if(m>n) {
            parallel = m;
            para_m = true;
        } else {
            parallel = n;
            para_m = false;
        }

        ParallelOperation::execute(
            parallel,kernel,para_m,trans,m,n,cond,kindC,x,kindX,y,kindY,out);
    }
};

}

extern "C" {
void rindow_matlib_s_where(
    int32_t trans,int32_t m,int32_t n,
    const uint8_t *cond, int32_t kindC,
    const float *x, int32_t kindX,
    const float *y, int32_t kindY,
    float *out)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Where<float>::execute(trans, m, n, cond, kindC, x, kindX, y, kindY, out);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_where(
    int32_t trans,int32_t m,int32_t n,
    const uint8_t *cond, int32_t kindC,
    const double *x, int32_t kindX,
    const double *y, int32_t kindY,
    double *out)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Where<double>::execute(trans, m, n, cond, kindC, x, kindX, y, kindY, out);
    RINDOW_END_CLEAR_EXCEPTION;
}
}