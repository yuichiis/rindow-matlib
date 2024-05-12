#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_log)( /* rindow_matlib_d_log */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_log _g_rindow_matlib_d_log = NULL;
void rindow_matlib_d_log(
    int32_t            n,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_log==NULL) {
        _g_rindow_matlib_d_log = rindow_load_rindowmatlib_func("rindow_matlib_d_log"); 
        if(_g_rindow_matlib_d_log==NULL) {
            printf("rindow_matlib_d_log not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_log(
        n,
        x,
        incX    
    );
}
