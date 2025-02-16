#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

// *** CAUTION ***
#define RINDOW_MATLIB_MASKING_TEMPLATE(n,k,len,i,j,fill,x,a) \
    for(int32_t h=0; h<k; h++) { \
        if(!x[i*k+h]) { \
            for(int l=0;l<len;l++) { \
                a[((i*n+j)*k+h)*len+l] =  fill; \
            } \
        } \
    } \

void rindow_matlib_s_masking(
    int32_t m,
    int32_t n,
    int32_t k,
    int32_t len,
    float fill,
    uint8_t *x, 
    float *a
) {
    if(m <= 0 || n <= 0 || k <=0) {
        return;
    }
    if(m==1 && n==1) {  // simple masking (same shape X and A)
        int32_t h;
        #pragma omp parallel for
        for(h=0; h<k; h++) {
            if(!x[h]) {
                for(int l=0;l<len;l++) {
                    a[h*len+l] = fill;
                }
            }
        }
    } else if(m==1 && n<k) {// broadcast for inner shape and n<k
        int32_t h;
        #pragma omp parallel for
        for(h=0; h<k; h++) {
            for(int32_t j=0; j<n; j++) {
                if(!x[h]) {
                    for(int l=0;l<len;l++) {
                        a[(j*k+h)*len+l] = fill;
                    }
                }
            }
        }
    } else {
        if(m>n) {
            int32_t i;
            #pragma omp parallel for
            for(i=0; i<m; i++) {
                for(int32_t j=0; j<n; j++) {
                    RINDOW_MATLIB_MASKING_TEMPLATE(n,k,len,i,j,fill,x,a)
                }
            }
        } else {
            int32_t j;
            #pragma omp parallel for
            for(j=0; j<n; j++) {
                for(int32_t i=0;i<m; i++) {
                    RINDOW_MATLIB_MASKING_TEMPLATE(n,k,len,i,j,fill,x,a)
                }
            }
        }
    }
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
    if(m <= 0 || n <= 0 || k <=0) {
        return;
    }
    if(m==1 && n==1) {  // simple masking (same shape X and A)
        int32_t h;
        #pragma omp parallel for
        for(h=0; h<k; h++) {
            if(!x[h]) {
                for(int l=0;l<len;l++) {
                    a[h*len+l] = fill;
                }
            }
        }
    } else if(m==1 && n<k) {// broadcast for inner shape and n<k
        int32_t h;
        #pragma omp parallel for
        for(h=0; h<k; h++) {
            for(int32_t j=0; j<n; j++) {
                if(!x[h]) {
                    for(int l=0;l<len;l++) {
                        a[(j*k+h)*len+l] = fill;
                    }
                }
            }
        }
    } else {
        if(m>n) {
            int32_t i;
            #pragma omp parallel for
            for(i=0; i<m; i++) {
                for(int32_t j=0; j<n; j++) {
                    RINDOW_MATLIB_MASKING_TEMPLATE(n,k,len,i,j,fill,x,a)
                }
            }
        } else {
            int32_t j;
            #pragma omp parallel for
            for(j=0; j<n; j++) {
                for(int32_t i=0;i<m; i++) {
                    RINDOW_MATLIB_MASKING_TEMPLATE(n,k,len,i,j,fill,x,a)
                }
            }
        }
    }
}
