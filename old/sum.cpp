#define NOMINMAX

#include "rindow/matlib.h"
#include <math.h>
#include "common.hpp"
#include <stdio.h>
#include <vector>
#include <algorithm>

namespace {

template <typename T>
class sumClass
{
public:
    using arg_sum_kernel_t = struct _arg_sum_kernel {
        #ifdef _MSC_VER
            HANDLE tid;
        #else
            pthread_t tid;
        #endif
            T sum;
            int32_t n;
            T *x;
            int32_t incX;
    };

    static void *sum_kernel(void *varg)
    {
        arg_sum_kernel_t *arg=(arg_sum_kernel_t *)varg;
        int32_t i;
        T sum=0;
        int32_t n = arg->n;
        T* x = arg->x;
        int32_t incX = arg->incX;
        for(i=0; i<arg->n; i++) {
            sum += x[i*incX];
        }
        arg->sum = sum;

        return NULL;
    }

    static T sum(int32_t n,T *x, int32_t incX)
    {
        if(n<=0) {
            return 0;
        }
        T sum = 0;
        int32_t nprocs=rindow_matlib_common_get_nprocs();
        int32_t num_thread=std::min(n,nprocs);
        int32_t cell_size = n/num_thread;
        int32_t remainder = n-cell_size*num_thread;
        printf("#num_thread=%d\n",num_thread);
        printf("#cell_size=%d\n",cell_size);
        printf("#remainder=%d\n",remainder);
        std::vector<arg_sum_kernel_t> th_arg(num_thread);
        for(int32_t i=0;i<num_thread;i++) {
            if(i==num_thread-1) {
                th_arg[i].n = cell_size+remainder;
            } else {
                th_arg[i].n = cell_size;
            }
            th_arg[i].x = &x[i*cell_size*incX];
            th_arg[i].incX = incX;
            th_arg[i].sum = 0.0;
            int rc = rindow_matlib_common_thread_create( &(th_arg[i].tid), NULL, &sum_kernel, &th_arg[i]);
            if(rc!=0) {
                return 0;
            }
        }
        for(int i=0;i<num_thread;i++) {
            int rc = rindow_matlib_common_thread_join(th_arg[i].tid,NULL);
            if(rc!=0) {
                return 0;
            }
            sum += th_arg[i].sum;
        }
        return sum;
    }
};

}


extern "C" {
float rindow_matlib_s_sum(int32_t n,float *x,int32_t incX)
{
    return sumClass<float>::sum(
        n,
        x,
        incX
    );
}

double rindow_matlib_d_sum(int32_t n,double *x,int32_t incX)
{
    return sumClass<double>::sum(
        n,
        x,
        incX
    );
}

}

/*
#define RINDOW_MATLIB_SUM_TEMPLATE(data_type) \
    for(i=0; i<n; i++) {\
        sum += pDataX[i*incX];\
    }

int64_t rindow_matlib_i_sum(int32_t dtype, int32_t n,void *x, int32_t incX)
{
    int32_t i;
    int64_t sum=0;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            int8_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(int8_t)
            break;
        }
        case rindow_matlib_dtype_uint8: {
            uint8_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(uint8_t)
            break;
        }
        case rindow_matlib_dtype_int16: {
            int16_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(int16_t)
            break;
        }
        case rindow_matlib_dtype_uint16: {
            uint16_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(uint16_t)
            break;
        }
        case rindow_matlib_dtype_int32: {
            int32_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(int32_t)
            break;
        }
        case rindow_matlib_dtype_uint32: {
            uint32_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(uint32_t)
            break;
        }
        case rindow_matlib_dtype_int64: {
            int64_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(int64_t)
            break;
        }
        case rindow_matlib_dtype_uint64: {
            uint64_t *pDataX=x;
            #pragma omp parallel for reduction(+:sum)
            RINDOW_MATLIB_SUM_TEMPLATE(uint64_t)
            break;
        }
        case rindow_matlib_dtype_bool: {
                uint8_t *pDataX=x;
                #pragma omp parallel for reduction(+:sum)
                for (i=0; i<n; i++) {
                    if(pDataX[i*incX]!=0) {
                        sum++;
                    }
                }
            break;
        }
    }
    return sum;
}
*/
