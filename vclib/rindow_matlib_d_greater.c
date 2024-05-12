#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_greater)( /* rindow_matlib_d_greater */
    int32_t            /* m */,
    int32_t            /* n */,
    double *            /* a */,
    int32_t            /* ldA */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_greater _g_rindow_matlib_d_greater = NULL;
void rindow_matlib_d_greater(
    int32_t            m,
    int32_t            n,
    double *            a,
    int32_t            ldA,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_greater==NULL) {
        _g_rindow_matlib_d_greater = rindow_load_rindowmatlib_func("rindow_matlib_d_greater"); 
        if(_g_rindow_matlib_d_greater==NULL) {
            printf("rindow_matlib_d_greater not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_greater(
        m,
        n,
        a,
        ldA,
        x,
        incX    
    );
}
