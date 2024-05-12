#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_isnan)( /* rindow_matlib_d_isnan */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_isnan _g_rindow_matlib_d_isnan = NULL;
void rindow_matlib_d_isnan(
    int32_t            n,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_isnan==NULL) {
        _g_rindow_matlib_d_isnan = rindow_load_rindowmatlib_func("rindow_matlib_d_isnan"); 
        if(_g_rindow_matlib_d_isnan==NULL) {
            printf("rindow_matlib_d_isnan not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_isnan(
        n,
        x,
        incX    
    );
}
