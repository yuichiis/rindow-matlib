#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;
using rindow::matlib::ParallelResults;

namespace {

template <typename T>
class Add
{
public:
    static void kernel(
        int32_t begin,
        int32_t end,
        bool para_m,
        int32_t trans,
        int32_t m,
        int32_t n,
        T alpha,
        T *x,
        int32_t incX,
        T *a,
        int32_t ldA
    )
    {
        int32_t begin_i,end_i,begin_j,end_j;
        if(para_m) {
            begin_i=begin;
            end_i=end;
            begin_j=0;
            end_j=n;
        } else {
            begin_i=0;
            end_i=m;
            begin_j=begin;
            end_j=end;
        }
        if(trans==RINDOW_MATLIB_NO_TRANS) {
            for(int32_t i=begin_i; i<end_i; i++) {
                for(int32_t j=begin_j; j<end_j; j++) {
                    a[i*ldA+j] += alpha*x[j*incX];
                }
            }
        } else {
            for(int32_t i=begin_i; i<end_i; i++) {
                for(int32_t j=begin_j; j<end_j; j++) {
                    a[i*ldA+j] += alpha*x[i*incX];
                }
            }
        }
    }

    static void execute(int32_t trans,int32_t m,int32_t n,T alpha, T *x, int32_t incX, T *a, int32_t ldA)
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

        ParallelResults<void> results;
        ParallelOperation::enqueue(parallel,results,kernel,para_m,trans,m,n,alpha,x,incX,a,ldA);

        for(auto && result: results) {
            result.get();
        }
    }
};

}

extern "C" {
void rindow_matlib_s_add(int32_t trans,int32_t m,int32_t n, float alpha, float *x, int32_t incX, float *a, int32_t ldA)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Add<float>::execute(trans, m, n, alpha, x, incX, a, ldA);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_add(int32_t trans,int32_t m,int32_t n, double alpha, double *x, int32_t incX, double *a, int32_t ldA)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Add<double>::execute(trans, m, n, alpha, x, incX, a, ldA);
    RINDOW_END_CLEAR_EXCEPTION;
}
}
