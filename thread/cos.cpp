#include "rindow/matlib.h"
#include "common.hpp"
#include <cmath>

using rindow::matlib::ParallelOperation;
using rindow::matlib::ParallelResults;

namespace {

template <typename T>
class Cos
{
public:
    static void kernel(
        int32_t begin,
        int32_t end,
        int32_t n,
        T *x,
        int32_t incX
    )
    {
        for(int32_t i = begin; i < end; i++) {
            x[i * incX] = std::cos(x[i * incX]);
        }
    }

    static void execute(int32_t n, T *x, int32_t incX)
    {
        if(n <= 0) {
            return;
        }

        ParallelOperation::execute(n,kernel,n,x,incX);
    }
};

}

extern "C" {
void rindow_matlib_s_cos(int32_t n, float *x, int32_t incX)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Cos<float>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_cos(int32_t n, double *x, int32_t incX)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Cos<double>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}

}
