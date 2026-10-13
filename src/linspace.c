#include "rindow/matlib.h"
#include <math.h>
#include <limits.h>
#include "common.h"

#define RINDOW_MATLIB_LINSPACE_FLOAT_TEMPLATE(T,TRANGE,x,i,incX,start,limit,delta) \
    { \
        TRANGE v = start + (TRANGE)i*delta; \
        if(delta>0) { \
            if(v>limit) { \
                v = limit; \
            } \
        } else { \
            if(v<limit) { \
                v = limit; \
            } \
        } \
        x[i*incX] = (T)v; \
    }

#define RINDOW_MATLIB_LINSPACE_INT_TEMPLATE(T,TRANGE,x,i,incX,start,limit,delta) \
    { \
        TRANGE v; \
        v = start + (TRANGE)i*delta; \
        if(delta>0) { \
            if(v>limit) { \
                v = limit; \
            } \
        } else { \
            if(v<limit) { \
                v = limit; \
            } \
        } \
        ((T *)x)[i*incX] = (T)v; \
    }


void rindow_matlib_s_range(
    int32_t n,float *x, int32_t incX, float start,float limit,float delta)
{
    if(delta>0) {
        limit = nextafterf(limit, -INFINITY);
    } else {
        limit = nextafterf(limit, INFINITY);
    }
    int32_t i;
    #pragma omp parallel for
    for(i=0; i<n; i++) {
        RINDOW_MATLIB_LINSPACE_FLOAT_TEMPLATE(float,float,x,i,incX,start,limit,delta)
    }
}

void rindow_matlib_d_range(
    int32_t n,double *x, int32_t incX, double start,double limit,double delta)
{
    if(delta>0) {
        limit = nextafter(limit, -INFINITY);
    } else {
        limit = nextafter(limit, INFINITY);
    }
    int32_t i;
    #pragma omp parallel for
    for(i=0; i<n; i++) {
        RINDOW_MATLIB_LINSPACE_FLOAT_TEMPLATE(double,double,x,i,incX,start,limit,delta)
    }
}

void rindow_matlib_i_range(int32_t dtype, int32_t n,void *x, int32_t incX, int64_t start, int64_t limit, int64_t delta)
{
    if(delta>0) {
        limit = limit - 1;
    } else {
        limit = limit + 1;
    }
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            int32_t i;
            #pragma omp parallel for
            for(i=0; i<n; i++) {
                RINDOW_MATLIB_LINSPACE_INT_TEMPLATE(int8_t,int64_t,x,i,incX,start,limit,delta)
            }
            break;
        }
        case rindow_matlib_dtype_uint8: {
            int32_t i;
            #pragma omp parallel for
            for(i=0; i<n; i++) {
                RINDOW_MATLIB_LINSPACE_INT_TEMPLATE(uint8_t,int64_t,x,i,incX,start,limit,delta)
            }
            break;
        }
        case rindow_matlib_dtype_int16: {
            int32_t i;
            #pragma omp parallel for
            for(i=0; i<n; i++) {
                RINDOW_MATLIB_LINSPACE_INT_TEMPLATE(int16_t,int64_t,x,i,incX,start,limit,delta)
            }
            break;
        }
        case rindow_matlib_dtype_uint16: {
            int32_t i;
            #pragma omp parallel for
            for(i=0; i<n; i++) {
                RINDOW_MATLIB_LINSPACE_INT_TEMPLATE(uint16_t,int64_t,x,i,incX,start,limit,delta)
            }
            break;
        }
        case rindow_matlib_dtype_int32: {
            int32_t i;
            #pragma omp parallel for
            for(i=0; i<n; i++) {
                RINDOW_MATLIB_LINSPACE_INT_TEMPLATE(int32_t,int64_t,x,i,incX,start,limit,delta)
            }
            break;
        }
        case rindow_matlib_dtype_uint32: {
            int32_t i;
            #pragma omp parallel for
            for(i=0; i<n; i++) {
                RINDOW_MATLIB_LINSPACE_INT_TEMPLATE(uint32_t,int64_t,x,i,incX,start,limit,delta)
            }
            break;
        }
        case rindow_matlib_dtype_int64: {
            int32_t i;
            #pragma omp parallel for
            for(i=0; i<n; i++) {
                RINDOW_MATLIB_LINSPACE_INT_TEMPLATE(int64_t,int64_t,x,i,incX,start,limit,delta)
            }
            break;
        }
        case rindow_matlib_dtype_uint64: {
            int32_t i;
            #pragma omp parallel for
            for(i=0; i<n; i++) {
                RINDOW_MATLIB_LINSPACE_INT_TEMPLATE(uint64_t,int64_t,x,i,incX,start,limit,delta)
            }
            break;
        }
    }
}

void rindow_matlib_s_linspace(
    int32_t n,float *x, int32_t incX, float start,float stop)
{
    float delta;
    if(n<=1) {
        delta = 0;
    } else {
        delta = (stop - start) / ((float)n-1);
    }
    int32_t i;
    #pragma omp parallel for
    for(i=0; i<n; i++) {
        RINDOW_MATLIB_LINSPACE_FLOAT_TEMPLATE(float,float,x,i,incX,start,stop,delta)
    }
}

void rindow_matlib_d_linspace(
    int32_t n,double *x, int32_t incX, double start,double stop)
{
    double delta;
    if(n<=1) {
        delta = 0;
    } else {
        delta = (stop - start) / ((double)n-1);
    }
    int32_t i;
    #pragma omp parallel for
    for(i=0; i<n; i++) {
        RINDOW_MATLIB_LINSPACE_FLOAT_TEMPLATE(double,double,x,i,incX,start,stop,delta)
    }
}

