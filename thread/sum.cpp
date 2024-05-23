#define NOMINMAX

#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;
using rindow::matlib::ParallelResults;

namespace {

template <typename T>
class SumClass
{
public:
    static T sum_kernel(
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

    static T sum_bool_kernel(
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

    static T sum(int32_t n, T *x, int32_t incX)
    {
        if(n <= 0) {
            return 0;
        }

        T total = 0;

        ParallelResults<T> results;
        ParallelOperation::execute(n,results,sum_kernel,n,x,incX);

        for(auto && result: results) {
            total += result.get();
        }

        return total;
    }

    static T sum_bool(int32_t n, T *x, int32_t incX)
    {
        if(n <= 0) {
            return 0;
        }

        T total = 0;

        ParallelResults<T> results;
        ParallelOperation::execute(n,results,sum_bool_kernel,n,x,incX);

        for(auto && result: results) {
            total += result.get();
        }

        return total;
    }
};

}

extern "C" {
float rindow_matlib_s_sum(int32_t n, float *x, int32_t incX)
{
    float ret;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    ret = SumClass<float>::sum(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
    return ret;
}

double rindow_matlib_d_sum(int32_t n, double *x, int32_t incX)
{
    double ret;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    ret = SumClass<double>::sum(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
    return ret;
}

int64_t rindow_matlib_i_sum(int32_t dtype, int32_t n,void *x, int32_t incX)
{
    int64_t ret=0;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            ret =(int64_t)SumClass<int8_t>::sum(n, (int8_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            ret =(int64_t)SumClass<uint8_t>::sum(n, (uint8_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int16: {
            ret =(int64_t)SumClass<int16_t>::sum(n, (int16_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            ret =(int64_t)SumClass<uint16_t>::sum(n, (uint16_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int32: {
            ret =(int64_t)SumClass<int32_t>::sum(n, (int32_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            ret =(int64_t)SumClass<uint32_t>::sum(n, (uint32_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_int64: {
            ret =(int64_t)SumClass<int64_t>::sum(n, (int64_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            ret =(int64_t)SumClass<uint64_t>::sum(n, (uint64_t*)x, incX);
            break;
        }
        case rindow_matlib_dtype_bool: {
            ret =(int64_t)SumClass<uint8_t>::sum(n, (uint8_t *)x, incX);
            break;
        }
    }
    RINDOW_END_CLEAR_EXCEPTION;
    return ret;
}

}
