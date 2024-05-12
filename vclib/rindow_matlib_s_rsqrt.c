#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_rsqrt)( /* rindow_matlib_s_rsqrt */
    int32_t            /* n */,
    float            /* alpha */,
    float *            /* x */,
    int32_t            /* incX */,
    float            /* beta */
);
static PFNrindow_matlib_s_rsqrt _g_rindow_matlib_s_rsqrt = NULL;
void rindow_matlib_s_rsqrt(
    int32_t            n,
    float            alpha,
    float *            x,
    int32_t            incX,
    float            beta
)
{
    if(_g_rindow_matlib_s_rsqrt==NULL) {
        _g_rindow_matlib_s_rsqrt = rindow_load_rindowmatlib_func("rindow_matlib_s_rsqrt"); 
        if(_g_rindow_matlib_s_rsqrt==NULL) {
            printf("rindow_matlib_s_rsqrt not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_rsqrt(
        n,
        alpha,
        x,
        incX,
        beta    
    );
}
