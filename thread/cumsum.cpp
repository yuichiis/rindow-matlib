#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;
using rindow::matlib::ParallelResults;

namespace {

template <typename T>
class Cumsum
{
public:
    static void kernel(
        int32_t begin,
        int32_t end,
        int32_t m,
        int32_t n,
        T *x, int32_t incX,
        int32_t exclusive,
        int32_t reverse,
        T *y, int32_t incY
    )
    {
        int32_t idxX,idxY;

        if(reverse) {
            idxX = begin*n*incX;
            idxY = end*n*incY - incY;
            incY = -incY;
        } else {
            idxX = begin*n*incX;
            idxY = begin*n*incY;
        }

        if(exclusive) {
            for(int32_t i=begin;i<end;i++) {
                T value = 0.0;
                for(int32_t j=0;j<n;j++,idxX+=incX,idxY+=incY) {
                    y[idxY] = value;
                    value += x[idxX];
                }
            }
        } else {
            for(int32_t i=begin;i<end;i++) {
                T value = 0.0;
                for(int32_t j=0;j<n;j++,idxX+=incX,idxY+=incY) {
                    value += x[idxX];
                    y[idxY] = value;
                }
            }
        }
    }

    static void execute(
        int32_t m,
        int32_t n,
        T *x, int32_t incX,
        int32_t exclusive,
        int32_t reverse,
        T *y, int32_t incY)
    {
        if(m <= 0 || n <= 0) {
            return;
        }

        ParallelOperation::execute(m,kernel,m,n,x,incX,exclusive,reverse,y,incY);
    }
};

}

extern "C" {
void rindow_matlib_s_cumsum(
    int32_t n,
    float *x, int32_t incX,
    int32_t exclusive,
    int32_t reverse,
    float *y, int32_t incY)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Cumsum<float>::execute(1,n,x,incX,exclusive,reverse,y,incY);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_cumsum(
    int32_t n,
    double *x, int32_t incX,
    int32_t exclusive,
    int32_t reverse,
    double *y, int32_t incY)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Cumsum<double>::execute(1,n,x,incX,exclusive,reverse,y,incY);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_s_cumsumb(
    int32_t m,
    int32_t n,
    float *x, int32_t incX,
    int32_t exclusive,
    int32_t reverse,
    float *y, int32_t incY)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Cumsum<float>::execute(m,n,x,incX,exclusive,reverse,y,incY);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_cumsumb(
    int32_t m,
    int32_t n,
    double *x, int32_t incX,
    int32_t exclusive,
    int32_t reverse,
    double *y, int32_t incY)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Cumsum<double>::execute(m,n,x,incX,exclusive,reverse,y,incY);
    RINDOW_END_CLEAR_EXCEPTION;
}

}

