#include "rindow/matlib.h"
#include "common.h"

// Column block width when parallelizing with axis=0
#define RINDOW_MATLIB_BINCOUNT_BLOCK 256

// axis=-1: input [m,n] -> output [m,numClasses]
// Clear row i to zero and count
#define RINDOW_MATLIB_BINCOUNT_LAST_TEMPLATE(TI,TO,input,output,i,n,numClasses,addMode) \
    {                                                                           \
        const TI *in_  = (input)  + (int64_t)(i) * (n);                         \
        TO       *out_ = (output) + (int64_t)(i) * (numClasses);                \
        if(!(addMode)) {                                                        \
            for(int32_t c=0; c<(numClasses); c++) {                             \
                out_[c] = 0;                                                    \
            }                                                                   \
        }                                                                       \
        for(int32_t j=0; j<(n); j++) {                                          \
            TI v = in_[j];                                                      \
            if(v >= 0 && v < (TI)(numClasses)) {                                \
                out_[v]++;                                                      \
            }                                                                   \
        }                                                                       \
    }

// axis=0: input [m,n] -> output [numClasses,n]
// Process column block b: clear to zero and count
#define RINDOW_MATLIB_BINCOUNT_FIRST_TEMPLATE(TI,TO,input,output,b,m,n,numClasses,addMode) \
    {                                                                              \
        int64_t j0_ = (int64_t)(b) * RINDOW_MATLIB_BINCOUNT_BLOCK;                 \
        int64_t j1_ = j0_ + RINDOW_MATLIB_BINCOUNT_BLOCK;                          \
        if(j1_ > (n)) {                                                            \
            j1_ = (n);                                                             \
        }                                                                          \
        if(!(addMode)) {                                                           \
            for(int32_t c=0; c<(numClasses); c++) {                                \
                TO *out_ = (output) + (int64_t)c * (n);                            \
                for(int64_t j=j0_; j<j1_; j++) {                                   \
                    out_[j] = 0;                                                   \
                }                                                                  \
            }                                                                      \
        }                                                                          \
        for(int32_t i=0; i<(m); i++) {                                             \
            const TI *in_ = (input) + (int64_t)i * (n);                            \
            for(int64_t j=j0_; j<j1_; j++) {                                       \
                TI v = in_[j];                                                     \
                if(v >= 0 && v < (TI)(numClasses)) {                               \
                    (output)[(int64_t)v * (n) + j]++;                              \
                }                                                                  \
            }                                                                      \
        }                                                                          \
    }

void rindow_matlib_i32_bincount(
    int32_t trans, int32_t m, int32_t n, int32_t numClasses, int32_t addMode,
    const int32_t *input, int32_t *output)
{
    if(m<=0 || n<=0 || numClasses<=0) {
        return;
    }
    if(trans==RINDOW_MATLIB_NO_TRANS) {
        int32_t i;
        #pragma omp parallel for
        for(i=0; i<m; i++) {
            RINDOW_MATLIB_BINCOUNT_LAST_TEMPLATE(int32_t,int32_t,input,output,i,n,numClasses,addMode)
        }
    } else {
        int32_t blocks = (int32_t)(((int64_t)n + RINDOW_MATLIB_BINCOUNT_BLOCK - 1) / RINDOW_MATLIB_BINCOUNT_BLOCK);
        int32_t b;
        #pragma omp parallel for
        for(b=0; b<blocks; b++) {
            RINDOW_MATLIB_BINCOUNT_FIRST_TEMPLATE(int32_t,int32_t,input,output,b,m,n,numClasses,addMode)
        }
    }
}

void rindow_matlib_i64_bincount(
    int32_t trans, int32_t m, int32_t n, int32_t numClasses, int32_t addMode,
    const int64_t *input, int64_t *output)
{
    if(m<=0 || n<=0 || numClasses<=0) {
        return;
    }
    if(trans==RINDOW_MATLIB_NO_TRANS) {
        int32_t i;
        #pragma omp parallel for
        for(i=0; i<m; i++) {
            RINDOW_MATLIB_BINCOUNT_LAST_TEMPLATE(int64_t,int64_t,input,output,i,n,numClasses,addMode)
        }
    } else {
        int32_t blocks = (int32_t)(((int64_t)n + RINDOW_MATLIB_BINCOUNT_BLOCK - 1) / RINDOW_MATLIB_BINCOUNT_BLOCK);
        int32_t b;
        #pragma omp parallel for
        for(b=0; b<blocks; b++) {
            RINDOW_MATLIB_BINCOUNT_FIRST_TEMPLATE(int64_t,int64_t,input,output,b,m,n,numClasses,addMode)
        }
    }
}
