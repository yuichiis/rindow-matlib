#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

int32_t rindow_matlib_s_iamin(int32_t n,float *x, int32_t incX)
{
    if(n <= 0) {
        return 0;
    }
    int32_t resultIdx = 0;
    float resultMin = fabsf(x[0]);
    #pragma omp parallel
    {
        int32_t i;
        float minVal=fabsf(x[0]);
        int32_t minIdx=0;
        #pragma omp for
        for(i=0; i<n; i++) {
            if(!isnan(minVal)) {
                float xi = fabsf(x[i*incX]);
                if(minVal>xi || isnan(xi)) {
                    minVal = xi;
                    minIdx = i;
                }
            }
        }
        #pragma omp critical
        {
            if(!isnan(resultMin)) {
                if(resultMin>minVal || isnan(minVal)) {
                    resultMin = minVal;
                    resultIdx = minIdx;
                }
            }
        }
    }
    return resultIdx;
}

int32_t rindow_matlib_d_iamin(int32_t n,double *x, int32_t incX)
{
    if(n <= 0) {
        return 0;
    }
    int32_t resultIdx = 0;
    double resultMin = fabs(x[0]);
    #pragma omp parallel
    {
        int32_t i;
        double minVal=fabs(x[0]);
        int32_t minIdx=0;
        #pragma omp for
        for(i=0; i<n; i++) {
            if(!isnan(minVal)) {
                double xi = fabs(x[i*incX]);
                if(minVal>xi || isnan(xi)) {
                    minVal = xi;
                    minIdx = i;
                }
            }
        }
        #pragma omp critical
        {
            if(!isnan(resultMin)) {
                if(resultMin>minVal || isnan(minVal)) {
                    resultMin = minVal;
                    resultIdx = minIdx;
                }
            }
        }
    }
    return resultIdx;
}

int32_t rindow_matlib_c_iamin(int32_t n,matlib_complex_float *x, int32_t incX)
{
    if(n <= 0) {
        return 0;
    }
    int32_t resultIdx = 0;
    float resultMin = sqrtf(x[0].real * x[0].real + x[0].imag * x[0].imag);
    #pragma omp parallel
    {
        int32_t i;
        float minVal=sqrtf(x[0].real * x[0].real + x[0].imag * x[0].imag);
        int32_t minIdx=0;
        #pragma omp for
        for(i=0; i<n; i++) {
            if(!isnan(minVal)) {
                float xi = sqrtf(x[i*incX].real * x[i*incX].real + x[i*incX].imag * x[i*incX].imag);
                if(minVal>xi || isnan(xi)) {
                    minVal = xi;
                    minIdx = i;
                }
            }
        }
        #pragma omp critical
        {
            if(!isnan(resultMin)) {
                if(resultMin>minVal || isnan(minVal)) {
                    resultMin = minVal;
                    resultIdx = minIdx;
                }
            }
        }
    }
    return resultIdx;
}

int32_t rindow_matlib_z_iamin(int32_t n,matlib_complex_double *x, int32_t incX)
{
    if(n <= 0) {
        return 0;
    }
    int32_t resultIdx = 0;
    double resultMin = sqrt(x[0].real * x[0].real + x[0].imag * x[0].imag);
    #pragma omp parallel
    {
        int32_t i;
        double minVal=sqrt(x[0].real * x[0].real + x[0].imag * x[0].imag);
        int32_t minIdx=0;
        #pragma omp for
        for(i=0; i<n; i++) {
            if(!isnan(minVal)) {
                double xi = sqrt(x[i*incX].real * x[i*incX].real + x[i*incX].imag * x[i*incX].imag);
                if(minVal>xi || isnan(xi)) {
                    minVal = xi;
                    minIdx = i;
                }
            }
        }
        #pragma omp critical
        {
            if(!isnan(resultMin)) {
                if(resultMin>minVal || isnan(minVal)) {
                    resultMin = minVal;
                    resultIdx = minIdx;
                }
            }
        }
    }
    return resultIdx;
}
