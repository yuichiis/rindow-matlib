#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;
using rindow::matlib::ParallelResults;

namespace {

template <typename T>
class Gathernd
{
public:
    static int32_t kernel(
        int32_t begin,
        int32_t end,
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t indexDepth,
        int32_t *paramShape,
        T *a,
        int32_t *x,
        T *b
    )
    {
        int32_t errcode = 0;
        for(int32_t i=begin; i<end; i++) {
            for(int32_t j=0; j<n; j++) {
                int32_t offset=0;
                int32_t multiplier=1;
                bool outofindex=false;
                for(int32_t h=indexDepth-1;h>=0;--h) {
                    int32_t selector = x[i*n*indexDepth + j*indexDepth + h];
                    if(selector<0||selector>=paramShape[h]) {
                        errcode = RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
                        outofindex=true;
                        break;
                    }
                    offset += selector*multiplier;
                    multiplier *= paramShape[h];
                }
                if(outofindex) {
                    continue;
                }
                int32_t selector = x[i];
                T *from = &a[i*multiplier*k + offset*k];
                T *to = &b[i*n*k + j*k];
                if(reverse) {
                    T *tmp;
                    tmp = from;
                    from = to;
                    to = tmp;
                }
                if(k==1) {
                    if(addMode) {
                        *to += *from;
                    } else {
                        *to = *from;
                    }
                } else {
                    if(addMode) {
                        for(int32_t idx=0; idx<k; idx++) {
                            to[idx] += from[idx];
                        }
                    } else {
                        for(int32_t idx=0; idx<k; idx++) {
                            to[idx] = from[idx];
                        }
                    }
                }
            }
        }
        return errcode;
    }

    static int32_t execute(
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t indexDepth,
        int32_t *paramShape,
        T *a,
        int32_t *x,
        T *b
    )
    {
        if(m <= 0 || n <= 0 || k <= 0) {
            return RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
        }
        int32_t value_width = sizeof(float);
    
        return ParallelOperation::reduceNotZero<int32_t>(m,kernel,reverse,addMode,m,n,k,indexDepth,paramShape,a,x,b);
    }
};

}

extern "C" {
int32_t rindow_matlib_s_gathernd(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t indexDepth,
    int32_t *paramShape,
    float *a,
    int32_t *x,
    float *b
)
{
    int32_t errcode;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Gathernd<float>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,a,x,b);
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_d_gathernd(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t indexDepth,
    int32_t *paramShape,
    double *a,
    int32_t *x,
    double *b
)
{
    int32_t errcode;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Gathernd<double>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,a,x,b);
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_i_gathernd(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t indexDepth,
    int32_t *paramShape,
    int32_t dtype,
    void *a,
    int32_t *x,
    void *b
)
{
    int32_t errcode;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    switch(dtype) {
        case rindow_matlib_dtype_int8: {
            errcode = Gathernd<int8_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(int8_t*)a,x,(int8_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            errcode = Gathernd<uint8_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(uint8_t*)a,x,(uint8_t*)b);
            break;
        }
        case rindow_matlib_dtype_int16: {
            errcode = Gathernd<int16_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(int16_t*)a,x,(int16_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            errcode = Gathernd<uint16_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(uint16_t*)a,x,(uint16_t*)b);
            break;
        }
        case rindow_matlib_dtype_int32: {
            errcode = Gathernd<int32_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(int32_t*)a,x,(int32_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            errcode = Gathernd<uint32_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(uint32_t*)a,x,(uint32_t*)b);
            break;
        }
        case rindow_matlib_dtype_int64: {
            errcode = Gathernd<int64_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(int64_t*)a,x,(int64_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            errcode = Gathernd<uint64_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(uint64_t*)a,x,(uint64_t*)b);
            break;
        }
        case rindow_matlib_dtype_bool: {
            errcode = Gathernd<uint8_t>::execute(reverse,addMode,m,n,k,indexDepth,paramShape,(uint8_t*)a,x,(uint8_t*)b);
            break;
        }
        default: {
            errcode = RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
        }
    }
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

}

