#include "rindow/matlib.h"
#include <math.h>
#include "common.h"

// *** CAUTION ***
#define RINDOW_MATLIB_MASKING_TEMPLATE(n,k,i,j,fill,x,a) \
    for(int32_t h=0; h<k; h++) { \
        if(!x[i*k+h]) { \
            a[(i*n+j)*k+h] =  fill; \
        } \
    } \

void rindow_matlib_s_masking(
    int32_t m,
    int32_t n,
    int32_t k,
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
                a[h] = fill;
            }
        }
    } else if(m==1 && n<k) {// broadcast for inner shape and n<k
        int32_t h;
        #pragma omp parallel for
        for(h=0; h<k; h++) {
            for(int32_t j=0; j<n; j++) {
                if(!x[h]) {
                    a[j*k+h] = fill;
                }
            }
        }
    } else {
        if(m>n) {
            int32_t i;
            #pragma omp parallel for
            for(i=0; i<m; i++) {
                for(int32_t j=0; j<n; j++) {
                    RINDOW_MATLIB_MASKING_TEMPLATE(n,k,i,j,fill,x,a)
                }
            }
        } else {
            int32_t j;
            #pragma omp parallel for
            for(j=0; j<n; j++) {
                for(int32_t i=0;i<m; i++) {
                    RINDOW_MATLIB_MASKING_TEMPLATE(n,k,i,j,fill,x,a)
                }
            }
        }
    }
}

void rindow_matlib_d_masking(
    int32_t m,
    int32_t n,
    int32_t k,
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
                a[h] = fill;
            }
        }
    } else if(m==1 && n<k) {// broadcast for inner shape and n<k
        int32_t h;
        #pragma omp parallel for
        for(h=0; h<k; h++) {
            for(int32_t j=0; j<n; j++) {
                if(!x[h]) {
                    a[j*k+h] = fill;
                }
            }
        }
    } else {
        if(m>n) {
            int32_t i;
            #pragma omp parallel for
            for(i=0; i<m; i++) {
                for(int32_t j=0; j<n; j++) {
                    RINDOW_MATLIB_MASKING_TEMPLATE(n,k,i,j,fill,x,a)
                }
            }
        } else {
            int32_t j;
            #pragma omp parallel for
            for(j=0; j<n; j++) {
                for(int32_t i=0;i<m; i++) {
                    RINDOW_MATLIB_MASKING_TEMPLATE(n,k,i,j,fill,x,a)
                }
            }
        }
    }
}
