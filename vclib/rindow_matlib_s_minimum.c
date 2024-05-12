#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_minimum)( /* rindow_matlib_s_minimum */
    int32_t            /* m */,
    int32_t            /* n */,
    float *            /* a */,
    int32_t            /* ldA */,
    float *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_s_minimum _g_rindow_matlib_s_minimum = NULL;
void rindow_matlib_s_minimum(
    int32_t            m,
    int32_t            n,
    float *            a,
    int32_t            ldA,
    float *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_s_minimum==NULL) {
        _g_rindow_matlib_s_minimum = rindow_load_rindowmatlib_func("rindow_matlib_s_minimum"); 
        if(_g_rindow_matlib_s_minimum==NULL) {
            printf("rindow_matlib_s_minimum not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_minimum(
        m,
        n,
        a,
        ldA,
        x,
        incX    
    );
}
