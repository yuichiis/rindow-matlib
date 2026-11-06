#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "rindow/matlib.h"
#include "common.hpp"
#include <limits>
#include <cmath>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
struct IAMinAbs
{
    using result_type = T;
    static result_type abs(T value)
    {
        return std::fabs(value);
    }
};

template <>
struct IAMinAbs<matlib_complex_float>
{
    using result_type = float;
    static result_type abs(matlib_complex_float value)
    {
        return std::sqrt(value.real * value.real + value.imag * value.imag);
    }
};

template <>
struct IAMinAbs<matlib_complex_double>
{
    using result_type = double;
    static result_type abs(matlib_complex_double value)
    {
        return std::sqrt(value.real * value.real + value.imag * value.imag);
    }
};

template <typename T>
struct _RIAMin {
    int32_t index;
    T value;
};

template <typename T>
using RIAMin = struct _RIAMin<T>;

template <typename T>
class IAMin
{
private:
    using TR = typename IAMinAbs<T>::result_type;

    static RIAMin<TR> kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        TR minVal=IAMinAbs<T>::abs(x[cell.begin*incX]);
        int32_t minIdx=cell.begin;
        if(std::isnan(minVal)) {
            RIAMin<TR> result = {minIdx,minVal};
            return result;
        }
        for(int32_t i = cell.begin+1; i < cell.end; i++) {
            TR xi = IAMinAbs<T>::abs(x[i*incX]);
            if(minVal>xi || std::isnan(xi)) {
                minVal = xi;
                minIdx = i;
            }
            if(std::isnan(xi)) {
                break;
            }
        }
        RIAMin<TR> result = {minIdx,minVal};
        return result;
    }

    static RIAMin<TR> compare(
        RIAMin<TR> initialValue,
        RIAMin<TR> value
    )
    {
        if(initialValue.index<0) {
            return value;
        }
        if(std::isnan(initialValue.value)) {
            return initialValue;
        } else if(initialValue.value>value.value || std::isnan(value.value)) {
            return value;
        } else {
            return initialValue;
        }
    }

public:
    static int32_t execute(
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        if(n <= 0) {
            return 0;
        }

        RIAMin<TR> initialValue = { -1, std::numeric_limits<TR>::max() };
        RIAMin<TR> result = ParallelOperation::reduce(n,compare,initialValue,kernel,n,x,incX);
        return result.index;
    }
};

}

extern "C" {
int32_t rindow_matlib_s_iamin(int32_t n, float *x, int32_t incX)
{
    int32_t ret;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    ret = IAMin<float>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
    return ret;
}

int32_t rindow_matlib_d_iamin(int32_t n, double *x, int32_t incX)
{
    int32_t ret;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    ret = IAMin<double>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
    return ret;
}

int32_t rindow_matlib_c_iamin(int32_t n, matlib_complex_float *x, int32_t incX)
{
    int32_t ret;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    ret = IAMin<matlib_complex_float>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
    return ret;
}

int32_t rindow_matlib_z_iamin(int32_t n, matlib_complex_double *x, int32_t incX)
{
    int32_t ret;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    ret = IAMin<matlib_complex_double>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
    return ret;
}
}
