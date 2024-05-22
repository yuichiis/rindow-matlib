#define NOMINMAX

#include "rindow/matlib.h"
#include <math.h>
#include "common.hpp"
#include <stdio.h>
#include <vector>
#include <algorithm>

using rindow::matlib::ThreadPool;

namespace {

template <typename T>
class sumClass
{
public:
    using arg_sum_kernel_t = struct _arg_sum_kernel {
        T sum;
        int32_t n;
        T *x;
        int32_t incX;
    };

    static T sum_kernel(arg_sum_kernel_t arg)
    {
        T sum = 0;
        T* x = arg.x;
        int32_t incX = arg.incX;
        for(int32_t i = 0; i < arg.n; i++) {
            sum += x[i * incX];
        }
        return sum;
    }

    static T sum(int32_t n, T *x, int32_t incX)
    {
        if(n <= 0) {
            return 0;
        }

        T sum = 0;
        int32_t nprocs = rindow_matlib_common_get_nprocs();
        int32_t num_thread = std::min(n, nprocs);
        int32_t cell_size = n / num_thread;
        int32_t remainder = n - cell_size * num_thread;

        printf("#num_thread=%d\n", num_thread);
        printf("#cell_size=%d\n", cell_size);
        printf("#remainder=%d\n", remainder);

        std::vector<arg_sum_kernel_t> args(num_thread);
        std::vector<std::future<T>> results;

        ThreadPool pool(num_thread);

        for(int32_t i = 0; i < num_thread; i++) {
            if(i == num_thread - 1) {
                args[i].n = cell_size + remainder;
            } else {
                args[i].n = cell_size;
            }
            args[i].x = &x[i * cell_size * incX];
            args[i].incX = incX;
            args[i].sum = 0.0;

            results.emplace_back(pool.enqueue(sum_kernel, args[i]));
        }

        for(auto && result: results) {
            sum += result.get();
        }

        return sum;
    }
};

}

extern "C" {
float rindow_matlib_s_sum(int32_t n, float *x, int32_t incX)
{
    return sumClass<float>::sum(n, x, incX);
}

double rindow_matlib_d_sum(int32_t n, double *x, int32_t incX)
{
    return sumClass<double>::sum(n, x, incX);
}

}
