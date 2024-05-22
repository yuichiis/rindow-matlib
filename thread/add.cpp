#define NOMINMAX

#include "rindow/matlib.h"
#include "common.hpp"
#include <vector>
#include <stdio.h>

using rindow::matlib::ThreadPool;

namespace {

template <typename T>
class addClass
{
public:
    using arg_add_kernel_t = struct _arg_add_kernel {
        int32_t begin;
        int32_t end;
        int32_t trans;
        int32_t m;
        int32_t n;
        T alpha;
        T *x;
        T *a;
        int32_t incX;
        int32_t ldA;
    };

    static void add_kernel(arg_add_kernel_t arg)
    {
        int32_t begin = arg.begin;
        int32_t end = arg.end;
        int32_t m = arg.m;
        int32_t n = arg.n;
        T alpha = arg.alpha;
        T* x = arg.x;
        int32_t incX = arg.incX;
        T* a = arg.a;
        int32_t ldA = arg.ldA;

        if(arg.trans==RINDOW_MATLIB_NO_TRANS) {
            for(int32_t i=begin; i<end; i++) {
                for(int32_t j=0; j<n; j++) {
                    a[i*ldA+j] += alpha*x[j*incX];
                }
            }
        } else {
            for(int32_t i=begin; i<end; i++) {
                for(int32_t j=0; j<n; j++) {
                    a[i*ldA+j] += alpha*x[i*incX];
                }
            }
        }
    }

    static void add(int32_t trans,int32_t m,int32_t n,T alpha, T *x, int32_t incX, T *a, int32_t ldA)
    {
        if(n <= 0 || m <= 0) {
            return;
        }

        ThreadPool& pool = ThreadPool::getInstance(0);
        int32_t max_threads = (int32_t)pool.getMaxThreads();

        int32_t num_thread = std::min(m, max_threads);
        int32_t cell_size = m / num_thread;
        int32_t remainder = m - cell_size * num_thread;

        printf("#m,n=%d,%d\n", m,n);
        printf("#num_thread=%d\n", num_thread);
        printf("#cell_size=%d\n", cell_size);
        printf("#remainder=%d\n", remainder);

        std::vector<arg_add_kernel_t> args(num_thread);
        std::vector<std::future<void>> results;


        for(int32_t i = 0; i < num_thread; i++) {
            args[i].begin = i * cell_size;
            if(i == num_thread - 1) {
                args[i].end = (i+1) * cell_size + remainder;
            } else {
                args[i].end = (i+1) * cell_size;
            }
            args[i].trans = trans;
            args[i].m = m;
            args[i].n = n;
            args[i].alpha = alpha;
            args[i].x = x;
            args[i].a = a;
            args[i].incX = incX;
            args[i].ldA = ldA;

            results.emplace_back(pool.enqueue(add_kernel, args[i]));
        }

        for(auto && result: results) {
            result.get();
        }
    }
};

}

extern "C" {
void rindow_matlib_s_add(int32_t trans,int32_t m,int32_t n, float alpha, float *x, int32_t incX, float *a, int32_t ldA)
{
    addClass<float>::add(trans, m, n, alpha, x, incX, a, ldA);
}

void rindow_matlib_d_add(int32_t trans,int32_t m,int32_t n, double alpha, double *x, int32_t incX, double *a, int32_t ldA)
{
    addClass<double>::add(trans, m, n, alpha, x, incX, a, ldA);
}
}
