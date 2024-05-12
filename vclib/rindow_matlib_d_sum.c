#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef double (CALLBACK* PFNrindow_matlib_d_sum)( /* rindow_matlib_d_sum */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_sum _g_rindow_matlib_d_sum = NULL;
double rindow_matlib_d_sum(
    int32_t            n,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_sum==NULL) {
        _g_rindow_matlib_d_sum = rindow_load_rindowmatlib_func("rindow_matlib_d_sum"); 
        if(_g_rindow_matlib_d_sum==NULL) {
            printf("rindow_matlib_d_sum not found.\n");
            return 0;
        }
    }
    return _g_rindow_matlib_d_sum(
        n,
        x,
        incX    
    );
}
