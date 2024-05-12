#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_multiply)( /* rindow_matlib_d_multiply */
    int32_t            /* trans */,
    int32_t            /* m */,
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */,
    double *            /* a */,
    int32_t            /* ldA */
);
static PFNrindow_matlib_d_multiply _g_rindow_matlib_d_multiply = NULL;
void rindow_matlib_d_multiply(
    int32_t            trans,
    int32_t            m,
    int32_t            n,
    double *            x,
    int32_t            incX,
    double *            a,
    int32_t            ldA
)
{
    if(_g_rindow_matlib_d_multiply==NULL) {
        _g_rindow_matlib_d_multiply = rindow_load_rindowmatlib_func("rindow_matlib_d_multiply"); 
        if(_g_rindow_matlib_d_multiply==NULL) {
            printf("rindow_matlib_d_multiply not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_multiply(
        trans,
        m,
        n,
        x,
        incX,
        a,
        ldA    
    );
}
