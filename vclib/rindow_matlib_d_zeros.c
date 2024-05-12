#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_zeros)( /* rindow_matlib_d_zeros */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_zeros _g_rindow_matlib_d_zeros = NULL;
void rindow_matlib_d_zeros(
    int32_t            n,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_zeros==NULL) {
        _g_rindow_matlib_d_zeros = rindow_load_rindowmatlib_func("rindow_matlib_d_zeros"); 
        if(_g_rindow_matlib_d_zeros==NULL) {
            printf("rindow_matlib_d_zeros not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_zeros(
        n,
        x,
        incX    
    );
}
