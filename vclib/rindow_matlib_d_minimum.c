#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_minimum)( /* rindow_matlib_d_minimum */
    int32_t            /* m */,
    int32_t            /* n */,
    double *            /* a */,
    int32_t            /* ldA */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_minimum _g_rindow_matlib_d_minimum = NULL;
void rindow_matlib_d_minimum(
    int32_t            m,
    int32_t            n,
    double *            a,
    int32_t            ldA,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_minimum==NULL) {
        _g_rindow_matlib_d_minimum = rindow_load_rindowmatlib_func("rindow_matlib_d_minimum"); 
        if(_g_rindow_matlib_d_minimum==NULL) {
            printf("rindow_matlib_d_minimum not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_minimum(
        m,
        n,
        a,
        ldA,
        x,
        incX    
    );
}
