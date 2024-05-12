#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef int32_t (CALLBACK* PFNrindow_matlib_d_imin)( /* rindow_matlib_d_imin */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_imin _g_rindow_matlib_d_imin = NULL;
int32_t rindow_matlib_d_imin(
    int32_t            n,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_imin==NULL) {
        _g_rindow_matlib_d_imin = rindow_load_rindowmatlib_func("rindow_matlib_d_imin"); 
        if(_g_rindow_matlib_d_imin==NULL) {
            printf("rindow_matlib_d_imin not found.\n");
            return 0;
        }
    }
    return _g_rindow_matlib_d_imin(
        n,
        x,
        incX    
    );
}
