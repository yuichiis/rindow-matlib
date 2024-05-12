#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_greater)( /* rindow_matlib_s_greater */
    int32_t            /* m */,
    int32_t            /* n */,
    float *            /* a */,
    int32_t            /* ldA */,
    float *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_s_greater _g_rindow_matlib_s_greater = NULL;
void rindow_matlib_s_greater(
    int32_t            m,
    int32_t            n,
    float *            a,
    int32_t            ldA,
    float *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_s_greater==NULL) {
        _g_rindow_matlib_s_greater = rindow_load_rindowmatlib_func("rindow_matlib_s_greater"); 
        if(_g_rindow_matlib_s_greater==NULL) {
            printf("rindow_matlib_s_greater not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_greater(
        m,
        n,
        a,
        ldA,
        x,
        incX    
    );
}
