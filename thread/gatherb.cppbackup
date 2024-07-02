#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Gatherb
{
public:
    static int32_t kernel(
        ParallelOperation::cellInfo cell,
        int32_t reverse,
        int32_t addMode,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t numClass,
        T *a,
        int32_t *x,
        T *b
    )
    {
        int32_t errcode = 0;
        for(int32_t i=cell.begin; i<cell.end; i++) {
            int32_t selector = x[i];
            if(selector<0||selector>=numClass) {
                errcode = RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
                continue;
            }
            for(int32_t j=0; j<n; j++) {
                T *from = &a[(i*n*numClass*k + j*numClass*k + selector*k)];
                T *to = &b[i*n*k + j*k]; \
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
        int32_t numClass,
        T *a,
        int32_t *x,
        T *b
    )
    {
        if(m <= 0 || n <= 0 || k <= 0) {
            return RINDOW_MATLIB_E_PERM_OUT_OF_RANGE;
        }
        int32_t value_width = sizeof(float);
    
        return ParallelOperation::reduceNotZero<int32_t>(m,kernel,reverse,addMode,m,n,k,numClass,a,x,b);
    }
};

}

extern "C" {
int32_t rindow_matlib_s_gatherb(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t numClass,
    float *a,
    int32_t *x,
    float *b
)
{
    int32_t errcode;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Gatherb<float>::execute(reverse,addMode,m,n,k,numClass,a,x,b);
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_d_gatherb(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t numClass,
    double *a,
    int32_t *x,
    double *b
)
{
    int32_t errcode;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Gatherb<double>::execute(reverse,addMode,m,n,k,numClass,a,x,b);
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_i_gatherb(
    int32_t reverse,
    int32_t addMode,
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t numClass,
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
            errcode = Gatherb<int8_t>::execute(reverse,addMode,m,n,k,numClass,(int8_t*)a,x,(int8_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint8: {
            errcode = Gatherb<uint8_t>::execute(reverse,addMode,m,n,k,numClass,(uint8_t*)a,x,(uint8_t*)b);
            break;
        }
        case rindow_matlib_dtype_int16: {
            errcode = Gatherb<int16_t>::execute(reverse,addMode,m,n,k,numClass,(int16_t*)a,x,(int16_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint16: {
            errcode = Gatherb<uint16_t>::execute(reverse,addMode,m,n,k,numClass,(uint16_t*)a,x,(uint16_t*)b);
            break;
        }
        case rindow_matlib_dtype_int32: {
            errcode = Gatherb<int32_t>::execute(reverse,addMode,m,n,k,numClass,(int32_t*)a,x,(int32_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint32: {
            errcode = Gatherb<uint32_t>::execute(reverse,addMode,m,n,k,numClass,(uint32_t*)a,x,(uint32_t*)b);
            break;
        }
        case rindow_matlib_dtype_int64: {
            errcode = Gatherb<int64_t>::execute(reverse,addMode,m,n,k,numClass,(int64_t*)a,x,(int64_t*)b);
            break;
        }
        case rindow_matlib_dtype_uint64: {
            errcode = Gatherb<uint64_t>::execute(reverse,addMode,m,n,k,numClass,(uint64_t*)a,x,(uint64_t*)b);
            break;
        }
        case rindow_matlib_dtype_bool: {
            errcode = Gatherb<uint8_t>::execute(reverse,addMode,m,n,k,numClass,(uint8_t*)a,x,(uint8_t*)b);
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

