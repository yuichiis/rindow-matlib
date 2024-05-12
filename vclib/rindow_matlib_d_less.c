#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_less)( /* rindow_matlib_d_less */
    int32_t            /* m */,
    int32_t            /* n */,
    double *            /* a */,
    int32_t            /* ldA */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_less _g_rindow_matlib_d_less = NULL;
void rindow_matlib_d_less(
    int32_t            m,
    int32_t            n,
    double *            a,
    int32_t            ldA,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_less==NULL) {
        _g_rindow_matlib_d_less = rindow_load_rindowmatlib_func("rindow_matlib_d_less"); 
        if(_g_rindow_matlib_d_less==NULL) {
            printf("rindow_matlib_d_less not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_less(
        m,
        n,
        a,
        ldA,
        x,
        incX    
    );
}
