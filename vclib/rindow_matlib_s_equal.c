#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_equal)( /* rindow_matlib_s_equal */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */,
    float *            /* y */,
    int32_t            /* incY */
);
static PFNrindow_matlib_s_equal _g_rindow_matlib_s_equal = NULL;
void rindow_matlib_s_equal(
    int32_t            n,
    float *            x,
    int32_t            incX,
    float *            y,
    int32_t            incY
)
{
    if(_g_rindow_matlib_s_equal==NULL) {
        _g_rindow_matlib_s_equal = rindow_load_rindowmatlib_func("rindow_matlib_s_equal"); 
        if(_g_rindow_matlib_s_equal==NULL) {
            printf("rindow_matlib_s_equal not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_equal(
        n,
        x,
        incX,
        y,
        incY    
    );
}
