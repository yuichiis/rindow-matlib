#include "rindow/matlib.h"
#include "common.h"
#include <string.h>
#include <stdio.h>

#define RINDOW_MATLIB_GATHERB_S_ADD_FUNC(k, from, to) \
    rindow_matlib_common_s_add(k, from, to)

#define RINDOW_MATLIB_GATHERB_D_ADD_FUNC(k, from, to) \
    rindow_matlib_common_d_add(k, from, to)

#define RINDOW_MATLIB_GATHERB_I_ADD_FUNC(k, from, to) \
    rindow_matlib_common_i_add(k, dtype, from, to)

#define RINDOW_MATLIB_GATHERB_S_SETADD_FUNC(to,from) *to += *from
#define RINDOW_MATLIB_GATHERB_D_SETADD_FUNC(to,from) *to += *from
#define RINDOW_MATLIB_GATHERB_I_SETADD_FUNC(to,from) rindow_matlib_common_i_add(1, dtype, from, to)
#define RINDOW_MATLIB_GATHERB_S_SETSET_FUNC(to,from) *to = *from
#define RINDOW_MATLIB_GATHERB_D_SETSET_FUNC(to,from) *to = *from
#define RINDOW_MATLIB_GATHERB_I_SETSET_FUNC(to,from) memcpy(to, from, value_width)

#define RINDOW_MATLIB_GATHERB_S_GETINC_FUNC(sum,b,pos) sum += b[pos]
#define RINDOW_MATLIB_GATHERB_D_GETINC_FUNC(sum,b,pos) sum += b[pos]
#define RINDOW_MATLIB_GATHERB_I_GETINC_FUNC(sum,b,pos) rindow_matlib_common_i_add(1, dtype, &b[(pos)*value_width], &sum)
#define RINDOW_MATLIB_GATHERB_S_SETINC_FUNC(a,pos,v) (a[pos] += v)
#define RINDOW_MATLIB_GATHERB_D_SETINC_FUNC(a,pos,v) (a[pos] += v)
#define RINDOW_MATLIB_GATHERB_I_SETINC_FUNC(a,pos,v) rindow_matlib_common_i_add(1,dtype,&v,&a[(pos)*value_width])

#define RINDOW_MATLIB_GATHERB_S_POSADD_FUNC(a,offA,b,offB,pos) a[(offA)+(pos)] += b[(offB)+(pos)]
#define RINDOW_MATLIB_GATHERB_D_POSADD_FUNC(a,offA,b,offB,pos) a[(offA)+(pos)] += b[(offB)+(pos)]
#define RINDOW_MATLIB_GATHERB_I_POSADD_FUNC(a,offA,b,offB,pos) \
    rindow_matlib_common_i_add(1, dtype, &b[(offB)+(pos)*value_width], &a[(offA)+(pos)*value_width])

//
// serial copy and addition algorithm
//
#define RINDOW_MATLIB_GATHERB_NORMAL_TEMPLATE(a,b,datatype,functype) \
    for(i=0;i<m;i++) { \
        int32_t selector = x[i]; \
        if(selector<0||selector>=numClass) { \
            errcode = RINDOW_MATLIB_E_PERM_OUT_OF_RANGE; \
            continue; \
        } \
        for(int32_t j=0; j<n; j++) { \
            datatype *from = &a[(i*n*numClass*ldB + j*numClass*ldB + selector*ldB)]; \
            datatype *to = &b[i*n*ldB + j*ldB]; \
            if(reverse) { \
                datatype *tmp; \
                tmp = from; \
                from = to; \
                to = tmp; \
            } \
            if(k==1) { \
                if(addMode) { \
                    RINDOW_MATLIB_GATHERB_##functype##_SETADD_FUNC(to,from); \
                } else { \
                    RINDOW_MATLIB_GATHERB_##functype##_SETSET_FUNC(to,from); \
                } \
            } else { \
                if(addMode) { \
                    RINDOW_MATLIB_GATHERB_##functype##_ADD_FUNC(k, from, to); \
                } else { \
                    memcpy(to, from,k*value_width); \
                } \
            } \
        } \
    }

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
    int32_t value_width = sizeof(float);
    int32_t errcode = 0;
    int32_t ldB = k;

    int32_t i;
    #pragma omp parallel for
    RINDOW_MATLIB_GATHERB_NORMAL_TEMPLATE(a,b,float,S)
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
    int32_t value_width = sizeof(double);
    int32_t errcode = 0;
    int32_t ldB = k;

    int32_t i;
    #pragma omp parallel for
    RINDOW_MATLIB_GATHERB_NORMAL_TEMPLATE(a,b,double,D)
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
    int32_t errcode = 0;
    int32_t ldB = k;

    if(!rindow_matlib_common_dtype_is_int(dtype) &&
        !rindow_matlib_common_dtype_is_bool(dtype)) {
        return RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
    }
    int32_t value_width = rindow_matlib_common_dtype_to_valuesize(dtype);
    if(value_width==0) {
        return RINDOW_MATLIB_E_UNSUPPORTED_DATA_TYPE;
    }
    ldB *= value_width;
    char *ax = a;
    char *bx = b;

    int32_t i;
    #pragma omp parallel for
    RINDOW_MATLIB_GATHERB_NORMAL_TEMPLATE(ax,bx,char,I)

    return errcode;
}
