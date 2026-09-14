#include "rindow/matlib.h"
#include "common.hpp"
#include <cmath>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T,typename TR>
class CAbs
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t n,
        T *x,
        int32_t incX,
        TR *y,
        int32_t incY
    )
    {
        for(int32_t i = cell.begin; i < cell.end; i++) {
            y[i * incY] = std::sqrt(x[i * incX].real * x[i * incX].real + x[i * incX].imag * x[i * incX].imag);
        }
    }

public:
    static void execute(int32_t n, T *x, int32_t incX, TR *y, int32_t incY)
    {
        if(n <= 0) {
            return;
        }

        ParallelOperation::execute(n,kernel,n,x,incX,y,incY);
    }
};

}

extern "C" {
void rindow_matlib_c_abs(int32_t n, matlib_complex_float *x, int32_t incX, float *y, int32_t incY)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    CAbs<matlib_complex_float,float>::execute(n, x, incX, y, incY);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_z_abs(int32_t n, matlib_complex_double *x, int32_t incX, double *y, int32_t incY)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    CAbs<matlib_complex_double,double>::execute(n, x, incX, y, incY);
    RINDOW_END_CLEAR_EXCEPTION;
}

}
