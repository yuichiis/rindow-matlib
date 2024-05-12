#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_sin)( /* rindow_matlib_d_sin */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_sin _g_rindow_matlib_d_sin = NULL;
void rindow_matlib_d_sin(
    int32_t            n,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_sin==NULL) {
        _g_rindow_matlib_d_sin = rindow_load_rindowmatlib_func("rindow_matlib_d_sin"); 
        if(_g_rindow_matlib_d_sin==NULL) {
            printf("rindow_matlib_d_sin not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_sin(
        n,
        x,
        incX    
    );
}
