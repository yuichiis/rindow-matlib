#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef int32_t (CALLBACK* PFNrindow_matlib_s_imin)( /* rindow_matlib_s_imin */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_s_imin _g_rindow_matlib_s_imin = NULL;
int32_t rindow_matlib_s_imin(
    int32_t            n,
    float *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_s_imin==NULL) {
        _g_rindow_matlib_s_imin = rindow_load_rindowmatlib_func("rindow_matlib_s_imin"); 
        if(_g_rindow_matlib_s_imin==NULL) {
            printf("rindow_matlib_s_imin not found.\n");
            return 0;
        }
    }
    return _g_rindow_matlib_s_imin(
        n,
        x,
        incX    
    );
}
