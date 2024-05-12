#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_not)( /* rindow_matlib_d_not */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_not _g_rindow_matlib_d_not = NULL;
void rindow_matlib_d_not(
    int32_t            n,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_not==NULL) {
        _g_rindow_matlib_d_not = rindow_load_rindowmatlib_func("rindow_matlib_d_not"); 
        if(_g_rindow_matlib_d_not==NULL) {
            printf("rindow_matlib_d_not not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_not(
        n,
        x,
        incX    
    );
}
