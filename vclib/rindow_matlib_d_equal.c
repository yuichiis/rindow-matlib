#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_equal)( /* rindow_matlib_d_equal */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */,
    double *            /* y */,
    int32_t            /* incY */
);
static PFNrindow_matlib_d_equal _g_rindow_matlib_d_equal = NULL;
void rindow_matlib_d_equal(
    int32_t            n,
    double *            x,
    int32_t            incX,
    double *            y,
    int32_t            incY
)
{
    if(_g_rindow_matlib_d_equal==NULL) {
        _g_rindow_matlib_d_equal = rindow_load_rindowmatlib_func("rindow_matlib_d_equal"); 
        if(_g_rindow_matlib_d_equal==NULL) {
            printf("rindow_matlib_d_equal not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_equal(
        n,
        x,
        incX,
        y,
        incY    
    );
}
