#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_less_equal)( /* rindow_matlib_d_less_equal */
    int32_t            /* m */,
    int32_t            /* n */,
    double *            /* a */,
    int32_t            /* ldA */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_less_equal _g_rindow_matlib_d_less_equal = NULL;
void rindow_matlib_d_less_equal(
    int32_t            m,
    int32_t            n,
    double *            a,
    int32_t            ldA,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_less_equal==NULL) {
        _g_rindow_matlib_d_less_equal = rindow_load_rindowmatlib_func("rindow_matlib_d_less_equal"); 
        if(_g_rindow_matlib_d_less_equal==NULL) {
            printf("rindow_matlib_d_less_equal not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_less_equal(
        m,
        n,
        a,
        ldA,
        x,
        incX    
    );
}
