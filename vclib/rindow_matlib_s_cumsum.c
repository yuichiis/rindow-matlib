#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_cumsum)( /* rindow_matlib_s_cumsum */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */,
    int32_t            /* exclusive */,
    int32_t            /* reverse */,
    float *            /* y */,
    int32_t            /* incY */
);
static PFNrindow_matlib_s_cumsum _g_rindow_matlib_s_cumsum = NULL;
void rindow_matlib_s_cumsum(
    int32_t            n,
    float *            x,
    int32_t            incX,
    int32_t            exclusive,
    int32_t            reverse,
    float *            y,
    int32_t            incY
)
{
    if(_g_rindow_matlib_s_cumsum==NULL) {
        _g_rindow_matlib_s_cumsum = rindow_load_rindowmatlib_func("rindow_matlib_s_cumsum"); 
        if(_g_rindow_matlib_s_cumsum==NULL) {
            printf("rindow_matlib_s_cumsum not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_cumsum(
        n,
        x,
        incX,
        exclusive,
        reverse,
        y,
        incY    
    );
}
