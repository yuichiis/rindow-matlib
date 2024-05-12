#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_sqrt)( /* rindow_matlib_d_sqrt */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_sqrt _g_rindow_matlib_d_sqrt = NULL;
void rindow_matlib_d_sqrt(
    int32_t            n,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_sqrt==NULL) {
        _g_rindow_matlib_d_sqrt = rindow_load_rindowmatlib_func("rindow_matlib_d_sqrt"); 
        if(_g_rindow_matlib_d_sqrt==NULL) {
            printf("rindow_matlib_d_sqrt not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_sqrt(
        n,
        x,
        incX    
    );
}
