#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

#define RINDOW_MATLIB_IAMIN_REAL(funcname, data_type, absfunc) \
int32_t funcname(int32_t n, data_type *x, int32_t incX) \
{ \
    if(n <= 0) { \
        return 0; \
    } \
    int32_t resultIdx = 0; \
    data_type resultMin = absfunc(x[0]); \
    _Pragma("omp parallel") \
    { \
        int32_t i; \
        data_type minVal = absfunc(x[0]); \
        int32_t minIdx = 0; \
        _Pragma("omp for") \
        for(i=0; i<n; i++) { \
            if(!isnan(minVal)) { \
                data_type xi = absfunc(x[i*incX]); \
                if(minVal>xi || isnan(xi)) { \
                    minVal = xi; \
                    minIdx = i; \
                } \
            } \
        } \
        _Pragma("omp critical") \
        { \
            if(!isnan(resultMin)) { \
                if(resultMin>minVal || isnan(minVal)) { \
                    resultMin = minVal; \
                    resultIdx = minIdx; \
                } \
            } \
        } \
    } \
    return resultIdx; \
}

#define RINDOW_MATLIB_IAMIN_COMPLEX(funcname, data_type, real_type, sqrtfunc) \
int32_t funcname(int32_t n, data_type *x, int32_t incX) \
{ \
    if(n <= 0) { \
        return 0; \
    } \
    int32_t resultIdx = 0; \
    real_type resultMin = sqrtfunc(x[0].real * x[0].real + x[0].imag * x[0].imag); \
    _Pragma("omp parallel") \
    { \
        int32_t i; \
        real_type minVal = sqrtfunc(x[0].real * x[0].real + x[0].imag * x[0].imag); \
        int32_t minIdx = 0; \
        _Pragma("omp for") \
        for(i=0; i<n; i++) { \
            if(!isnan(minVal)) { \
                real_type xi = sqrtfunc(x[i*incX].real * x[i*incX].real + x[i*incX].imag * x[i*incX].imag); \
                if(minVal>xi || isnan(xi)) { \
                    minVal = xi; \
                    minIdx = i; \
                } \
            } \
        } \
        _Pragma("omp critical") \
        { \
            if(!isnan(resultMin)) { \
                if(resultMin>minVal || isnan(minVal)) { \
                    resultMin = minVal; \
                    resultIdx = minIdx; \
                } \
            } \
        } \
    } \
    return resultIdx; \
}

RINDOW_MATLIB_IAMIN_REAL(rindow_matlib_s_iamin, float, fabsf)
RINDOW_MATLIB_IAMIN_REAL(rindow_matlib_d_iamin, double, fabs)
RINDOW_MATLIB_IAMIN_COMPLEX(rindow_matlib_c_iamin, matlib_complex_float, float, sqrtf)
RINDOW_MATLIB_IAMIN_COMPLEX(rindow_matlib_z_iamin, matlib_complex_double, double, sqrt)

#undef RINDOW_MATLIB_IAMIN_REAL
#undef RINDOW_MATLIB_IAMIN_COMPLEX
