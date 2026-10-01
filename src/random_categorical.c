#include <math.h>
#include <stdlib.h>
#include "rindow/matlib.h"
#include "common.h"

/* Uniform random number in the range (0, 1). */
#define RINDOW_MATLIB_UNIFORM(T) \
    ((T)((rindow_matlib_common_rand()%(RINDOW_MATLIB_COMMON_RAND_MAX-1))+1)/(T)RINDOW_MATLIB_COMMON_RAND_MAX)

/*
 * Sampling from categorical distribution (float/double common implementation)
 *   logits: [batchSize, numClasses] Unnormalized log probabilities
 *   y     : [batchSize, numSamples] Sampled class numbers
 *
 * Procedure (per batch):
 *   1. Subtract max to prevent overflow and create cumulative sum (CDF).
 *   2. Use u * total_sum as random number and determine class by binary search of CDF.
 */
#define RINDOW_MATLIB_RANDOMCATEGORICAL_IMPL(FUNC_NAME, T, EXP_FN) \
void FUNC_NAME( \
    int32_t batchSize, \
    int32_t numClasses, \
    int32_t numSamples, \
    const T *logits, \
    int32_t *y, \
    uint64_t seed \
) \
{ \
    if(seed!=0) { \
        rindow_matlib_common_srand((int32_t)(seed ^ (seed>>32))); \
    } \
    if(batchSize<=0 || numClasses<=0 || numSamples<=0) { \
        return; \
    } \
    T *cdf = (T *)malloc(sizeof(T)*(size_t)numClasses); \
    for(int32_t b=0; b<batchSize; b++) { \
        const T *row = &logits[(size_t)b*numClasses]; \
        int32_t *out = &y[(size_t)b*numSamples]; \
        T maxv = row[0]; \
        for(int32_t k=1; k<numClasses; k++) { \
            if(row[k]>maxv) { \
                maxv = row[k]; \
            } \
        } \
        if(cdf!=NULL) { \
            T sum = 0; \
            for(int32_t k=0; k<numClasses; k++) { \
                sum += EXP_FN(row[k]-maxv); \
                cdf[k] = sum; \
            } \
            for(int32_t s=0; s<numSamples; s++) { \
                T target = RINDOW_MATLIB_UNIFORM(T)*sum; \
                int32_t lo = 0; \
                int32_t hi = numClasses-1; \
                while(lo<hi) { \
                    int32_t mid = lo+(hi-lo)/2; \
                    if(cdf[mid]>target) { \
                        hi = mid; \
                    } else { \
                        lo = mid+1; \
                    } \
                } \
                out[s] = lo; \
            } \
        } else { \
            /* Fallback path: slow path when memory allocation fails */ \
            T sum = 0; \
            for(int32_t k=0; k<numClasses; k++) { \
                sum += EXP_FN(row[k]-maxv); \
            } \
            for(int32_t s=0; s<numSamples; s++) { \
                T target = RINDOW_MATLIB_UNIFORM(T)*sum; \
                T acc = 0; \
                int32_t sel = numClasses-1; \
                for(int32_t k=0; k<numClasses; k++) { \
                    acc += EXP_FN(row[k]-maxv); \
                    if(acc>target) { \
                        sel = k; \
                        break; \
                    } \
                } \
                out[s] = sel; \
            } \
        } \
    } \
    free(cdf); \
}

RINDOW_MATLIB_RANDOMCATEGORICAL_IMPL(rindow_matlib_s_randomcategorical, float,  expf)
RINDOW_MATLIB_RANDOMCATEGORICAL_IMPL(rindow_matlib_d_randomcategorical, double, exp)
