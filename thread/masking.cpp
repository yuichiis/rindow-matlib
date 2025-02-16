#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class Masking
{
private:
    static void kernel_simple(
        ParallelOperation::cellInfo cell,
        T fill,
        uint8_t *x,
        T *a
    )
    {
        int32_t begin_i,end_i;
        begin_i=cell.begin;
        end_i=cell.end;

        for(int32_t i=begin_i; i<end_i; i++) {
            if(!x[i]) {
                a[i] =  fill;
            }
        }
    }

    static void kernel_broadcast(
        ParallelOperation::cellInfo cell,
        bool para_m,
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t len,
        T fill,
        uint8_t *x,
        T *a
    )
    {
        int32_t begin_i,end_i,begin_j,end_j;
        if(para_m) {
            begin_i=cell.begin;
            end_i=cell.end;
            begin_j=0;
            end_j=n;
        } else {
            begin_i=0;
            end_i=m;
            begin_j=cell.begin;
            end_j=cell.end;
        }

        if(k==1) {
            for(int32_t i=begin_i; i<end_i; i++) {
                for(int32_t j=begin_j; j<end_j; j++) {
                    if(!x[i]) {
                        for(int32_t l=0; l<len; l++) {
                            a[(i*n+j)*len+l] =  fill;
                        }
                    }
                }
            }
        } else {
            for(int32_t i=begin_i; i<end_i; i++) {
                for(int32_t j=begin_j; j<end_j; j++) {
                    for(int32_t h=0; h<k; h++) {
                        if(!x[i*k+h]) {
                            for(int32_t l=0; l<len; l++) {
                                a[((i*n+j)*k+h)*len+l] =  fill;
                            }
                        }
                    }
                }
            }
        }
    }

    static void kernel_broadcast_inner_ngtk(
        ParallelOperation::cellInfo cell,
        int32_t n,
        int32_t k,
        int32_t len,
        T fill,
        uint8_t *x,
        T *a
    )
    {
        int32_t begin_h,end_h;
        begin_h=cell.begin;
        end_h=cell.end;

        for(int32_t j=0; j<n; j++) {
            for(int32_t h=begin_h; h<end_h; h++) {
                if(!x[h]) {
                    for(int32_t l=0; l<len; l++) {
                        a[(j*k+h)*len+l] = fill;
                    }
                }
            }
        }
    }

public:
    static void execute(
        int32_t m,
        int32_t n,
        int32_t k,
        int32_t len,
        T fill,
        uint8_t *x, 
        T *a
        )
    {
        if(m <= 0 || n <= 0 || k <=0) {
            return;
        }

        if(m==1 && n==1 && len==1) { // simple masking (same shape X and A)
            ParallelOperation::execute(k,kernel_simple,fill,x,a);
        } else if(m==1 && n<k) {
            ParallelOperation::execute(k,kernel_broadcast_inner_ngtk,n,k,len,fill,x,a);
        } else {
            int32_t parallel;
            bool para_m;
            if(m>n) {
                parallel = m;
                para_m = true;
            } else {
                parallel = n;
                para_m = false;
            }
            ParallelOperation::execute(parallel,kernel_broadcast,para_m,m,n,k,len,fill,x,a);
        }
    }
};

}

extern "C" {
void rindow_matlib_s_masking(
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t len,
    float fill,
    uint8_t *x, 
    float *a
) {
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Masking<float>::execute(m, n, k, len, fill, x, a);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_masking(
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t len,
    double fill,
    uint8_t *x, 
    double *a
) {
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Masking<double>::execute(m, n, k, len, fill, x, a);
    RINDOW_END_CLEAR_EXCEPTION;
}
}
