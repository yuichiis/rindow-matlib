#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_maximum)( /* rindow_matlib_s_maximum */
    int32_t            /* m */,
    int32_t            /* n */,
    float *            /* a */,
    int32_t            /* ldA */,
    float *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_s_maximum _g_rindow_matlib_s_maximum = NULL;
void rindow_matlib_s_maximum(
    int32_t            m,
    int32_t            n,
    float *            a,
    int32_t            ldA,
    float *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_s_maximum==NULL) {
        _g_rindow_matlib_s_maximum = rindow_load_rindowmatlib_func("rindow_matlib_s_maximum"); 
        if(_g_rindow_matlib_s_maximum==NULL) {
            printf("rindow_matlib_s_maximum not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_maximum(
        m,
        n,
        a,
        ldA,
        x,
        incX    
    );
}
