#include "rindow/matlib.h"
#include "common.h"
#include <math.h>

#define RINDOW_MATLIB_CABS_TEMPLATE(x,i,incX,y,incY,sqrtfunc) \
    y[i * incY] = sqrtfunc(x[i * incX].real * x[i * incX].real + x[i * incX].imag * x[i * incX].imag);


void rindow_matlib_c_abs(int32_t n, matlib_complex_float *x, int32_t incX, float *y, int32_t incY)
{
    int32_t i;
    #pragma omp parallel for
    for(i=0;i<n;i++) {
        RINDOW_MATLIB_CABS_TEMPLATE(x,i,incX,y,incY,sqrtf)
    }
}

void rindow_matlib_z_abs(int32_t n, matlib_complex_double *x, int32_t incX, double *y, int32_t incY)
{
    int32_t i;
    #pragma omp parallel for
    for(i=0;i<n;i++) {
        RINDOW_MATLIB_CABS_TEMPLATE(x,i,incX,y,incY,sqrt)
    }
}
