#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_cos)( /* rindow_matlib_d_cos */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_cos _g_rindow_matlib_d_cos = NULL;
void rindow_matlib_d_cos(
    int32_t            n,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_cos==NULL) {
        _g_rindow_matlib_d_cos = rindow_load_rindowmatlib_func("rindow_matlib_d_cos"); 
        if(_g_rindow_matlib_d_cos==NULL) {
            printf("rindow_matlib_d_cos not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_cos(
        n,
        x,
        incX    
    );
}
