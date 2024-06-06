#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;
using rindow::matlib::ParallelResults;

namespace {

template <typename T>
class Sum
{
public:
    static T kernel(
        int32_t begin,
        int32_t end,
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        T sum = 0;
        for(int32_t i = begin; i < end; i++) {
            sum += x[i * incX];
        }
        return sum;
    }

    static T bool_kernel(
        int32_t begin,
        int32_t end,
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        T sum = 0;
        for(int32_t i = begin; i < end; i++) {
            if(x[i * incX]!=0) {
                sum += 1;
            }
        }
        return sum;
    }

    static T execute(int32_t n, T *x, int32_t incX)
    {
        if(n <= 0) {
            return 0;
        }

        return ParallelOperation::reduceSum<T>(n,kernel,n,x,incX);
    }

    static T execute_bool(int32_t n, T *x, int32_t incX)
    {
        if(n <= 0) {
            return 0;
        }

        T total = 0;

        return ParallelOperation::reduceSum<T>(n,kernel,n,x,incX);
    }
};

}

extern "C" {
float rindow_matlib_s_sum(int32_t n, float *x, int32_t incX)
{
    float ret;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    ret = Sum<float>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
    return ret;
}

double rindow_matlib_d_sum(int32_t n, double *x, int32_t incX)
{
    double ret;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    ret = Sum<double>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
    return ret;
}

int64_t rindow_matlib_i_sum(int32_t dtype, int32_t n,void *x, int32_t incX)
{
    int64_t ret=0;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            ret =(int64_t)Sum<int8_t>::execute(n, (int8_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            ret =(int64_t)Sum<uint8_t>::execute(n, (uint8_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int16: {
            ret =(int64_t)Sum<int16_t>::execute(n, (int16_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            ret =(int64_t)Sum<uint16_t>::execute(n, (uint16_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int32: {
            ret =(int64_t)Sum<int32_t>::execute(n, (int32_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            ret =(int64_t)Sum<uint32_t>::execute(n, (uint32_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int64: {
            ret =(int64_t)Sum<int64_t>::execute(n, (int64_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            ret =(int64_t)Sum<uint64_t>::execute(n, (uint64_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_bool: {
            ret =(int64_t)Sum<uint8_t>::execute_bool(n, (uint8_t *)x, incX);
            break;
        }
    }
    RINDOW_END_CLEAR_EXCEPTION;
    return ret;
}

}
