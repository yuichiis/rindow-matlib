#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_tanh)( /* rindow_matlib_d_tanh */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_tanh _g_rindow_matlib_d_tanh = NULL;
void rindow_matlib_d_tanh(
    int32_t            n,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_tanh==NULL) {
        _g_rindow_matlib_d_tanh = rindow_load_rindowmatlib_func("rindow_matlib_d_tanh"); 
        if(_g_rindow_matlib_d_tanh==NULL) {
            printf("rindow_matlib_d_tanh not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_tanh(
        n,
        x,
        incX    
    );
}
