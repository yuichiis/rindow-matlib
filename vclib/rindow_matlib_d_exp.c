#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_exp)( /* rindow_matlib_d_exp */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_exp _g_rindow_matlib_d_exp = NULL;
void rindow_matlib_d_exp(
    int32_t            n,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_exp==NULL) {
        _g_rindow_matlib_d_exp = rindow_load_rindowmatlib_func("rindow_matlib_d_exp"); 
        if(_g_rindow_matlib_d_exp==NULL) {
            printf("rindow_matlib_d_exp not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_exp(
        n,
        x,
        incX    
    );
}
