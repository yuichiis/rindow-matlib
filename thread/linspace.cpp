#include "rindow/matlib.h"
#include "common.hpp"
#include <string.h>
#include <complex>
#include <type_traits>
#include <limits>
#include <cmath>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T, typename TRANGE>
class Linspace
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX,
        TRANGE start,
        TRANGE stop,
        TRANGE delta
    )
    {
        for(int32_t i = cell.begin; i < cell.end; i++) {
            TRANGE v = start + (TRANGE)i*delta;
            if(stop>start) {
                if(v>stop) {
                    v = stop;
                }
            } else {
                if(v<stop) {
                    v = stop;
                }
            }
            x[i*incX] = (T)v;
        }
    }

public:
    static void execute(int32_t n, T *x, int32_t incX, TRANGE start, TRANGE limit, TRANGE delta)
    {
        if(n <= 0) {
            return;
        }

        ParallelOperation::execute(n,kernel,n,x,incX,start,limit,delta);
    }
};

}

extern "C" {
void rindow_matlib_s_range(int32_t n, float *x, int32_t incX, float start, float limit, float delta)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    if(delta>0) {
        limit = nextafterf(limit, -INFINITY);
    } else {
        limit = nextafterf(limit, INFINITY);
    }
    Linspace<float, float>::execute(n, x, incX, start, limit, delta);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_range(int32_t n, double *x, int32_t incX, double start, double limit, double delta)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    if(delta>0) {
        limit = nextafter(limit, -INFINITY);
    } else {
        limit = nextafter(limit, INFINITY);
    }
    Linspace<double, double>::execute(n, x, incX, start, limit, delta);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_i_range(int32_t dtype, int32_t n,void *x, int32_t incX, int64_t start, int64_t limit, int64_t delta)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    if(delta>0) {
        limit = limit - 1;
    } else {
        limit = limit + 1;
    }
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            Linspace<int8_t,int64_t>::execute(n, (int8_t*)x, incX, start, limit, delta);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            Linspace<uint8_t,int64_t>::execute(n, (uint8_t*)x, incX, start, limit, delta);
            break;
        }
        case rindow_matlib_dtype_int16: {
            Linspace<int16_t,int64_t>::execute(n, (int16_t*)x, incX, start, limit, delta);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            Linspace<uint16_t,int64_t>::execute(n, (uint16_t*)x, incX, start, limit, delta);
            break;
        }
        case rindow_matlib_dtype_int32: {
            Linspace<int32_t,int64_t>::execute(n, (int32_t*)x, incX, start, limit, delta);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            Linspace<uint32_t,int64_t>::execute(n, (uint32_t*)x, incX, start, limit, delta);
            break;
        }
        case rindow_matlib_dtype_int64: {
            Linspace<int64_t,int64_t>::execute(n, (int64_t*)x, incX, start, limit, delta);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            Linspace<uint64_t,int64_t>::execute(n, (uint64_t*)x, incX, start, limit, delta);
            break;
        }
    }
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_s_linspace(int32_t n, float *x, int32_t incX, float start, float stop)
{
    float delta;
    if(n<=1) {
        delta = 0;
    } else {
        delta = (stop - start) / ((float)n-1);
    }
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Linspace<float, float>::execute(n, x, incX, start, stop, delta);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_linspace(int32_t n, double *x, int32_t incX, double start, double stop)
{
    double delta;
    if(n<=1) {
        delta = 0;
    } else {
        delta = (stop - start) / ((double)n-1);
    }
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Linspace<double, double>::execute(n, x, incX, start, stop, delta);
    RINDOW_END_CLEAR_EXCEPTION;
}

}
