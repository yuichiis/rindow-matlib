#include "rindow/matlib.h"
#include "common.hpp"
#include <cmath>

using rindow::matlib::ParallelOperation;
using rindow::matlib::ParallelResults;

namespace {

template <typename T>
class Exp
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
            x[i * incX] = std::exp(x[i * incX]);
        }
    }

    static void execute(int32_t n, T *x, int32_t incX)
    {
        if(n <= 0) {
            return;
        }

        ParallelResults<void> results;
        ParallelOperation::enqueue(n,results,kernel,n,x,incX);

        for(auto && result: results) {
            result.get();
        }
    }
};

}

extern "C" {
void rindow_matlib_s_exp(int32_t n, float *x, int32_t incX)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Exp<float>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_exp(int32_t n, double *x, int32_t incX)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Exp<double>::execute(n, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}

}
