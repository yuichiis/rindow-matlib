#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;
using rindow::matlib::ParallelResults;

namespace {

template <typename T>
class Fill
{
public:
    static void kernel(
        int32_t begin,
        int32_t end,
        int32_t dtype,
        int32_t n,
        T *value,
        T *x,
        int32_t incX
    )
    {
        size_t value_size = rindow_matlib_common_dtype_to_valuesize(dtype);
        char *vx = (char*)x;
        size_t step = incX*value_size;

        for(int32_t i=begin;i<end;i++) {
            memcpy(&(vx[i*step]),value,value_size);
        }
    }

    static void execute(int32_t dtype, int32_t n, T *value, T *x, int32_t incX)
    {
        if(n <= 0) {
            return;
        }

        ParallelResults<void> results;
        ParallelOperation::enqueue(n,results,kernel,dtype,n,value,x,incX);

        for(auto && result: results) {
            result.get();
        }
    }
};

}

extern "C" {
void rindow_matlib_fill(int32_t dtype, int32_t n, void *value, void *x, int32_t incX)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Fill<void>::execute(dtype, n, value, x, incX);
    RINDOW_END_CLEAR_EXCEPTION;
}

}
