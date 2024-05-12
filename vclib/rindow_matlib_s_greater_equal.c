#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_greater_equal)( /* rindow_matlib_s_greater_equal */
    int32_t            /* m */,
    int32_t            /* n */,
    float *            /* a */,
    int32_t            /* ldA */,
    float *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_s_greater_equal _g_rindow_matlib_s_greater_equal = NULL;
void rindow_matlib_s_greater_equal(
    int32_t            m,
    int32_t            n,
    float *            a,
    int32_t            ldA,
    float *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_s_greater_equal==NULL) {
        _g_rindow_matlib_s_greater_equal = rindow_load_rindowmatlib_func("rindow_matlib_s_greater_equal"); 
        if(_g_rindow_matlib_s_greater_equal==NULL) {
            printf("rindow_matlib_s_greater_equal not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_greater_equal(
        m,
        n,
        a,
        ldA,
        x,
        incX    
    );
}
