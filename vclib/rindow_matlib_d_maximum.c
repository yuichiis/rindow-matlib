#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_maximum)( /* rindow_matlib_d_maximum */
    int32_t            /* m */,
    int32_t            /* n */,
    double *            /* a */,
    int32_t            /* ldA */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_maximum _g_rindow_matlib_d_maximum = NULL;
void rindow_matlib_d_maximum(
    int32_t            m,
    int32_t            n,
    double *            a,
    int32_t            ldA,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_maximum==NULL) {
        _g_rindow_matlib_d_maximum = rindow_load_rindowmatlib_func("rindow_matlib_d_maximum"); 
        if(_g_rindow_matlib_d_maximum==NULL) {
            printf("rindow_matlib_d_maximum not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_maximum(
        m,
        n,
        a,
        ldA,
        x,
        incX    
    );
}
