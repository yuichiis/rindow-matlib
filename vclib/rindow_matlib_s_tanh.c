#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_tanh)( /* rindow_matlib_s_tanh */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_s_tanh _g_rindow_matlib_s_tanh = NULL;
void rindow_matlib_s_tanh(
    int32_t            n,
    float *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_s_tanh==NULL) {
        _g_rindow_matlib_s_tanh = rindow_load_rindowmatlib_func("rindow_matlib_s_tanh"); 
        if(_g_rindow_matlib_s_tanh==NULL) {
            printf("rindow_matlib_s_tanh not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_tanh(
        n,
        x,
        incX    
    );
}
