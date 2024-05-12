#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef int32_t (CALLBACK* PFNrindow_matlib_s_imax)( /* rindow_matlib_s_imax */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_s_imax _g_rindow_matlib_s_imax = NULL;
int32_t rindow_matlib_s_imax(
    int32_t            n,
    float *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_s_imax==NULL) {
        _g_rindow_matlib_s_imax = rindow_load_rindowmatlib_func("rindow_matlib_s_imax"); 
        if(_g_rindow_matlib_s_imax==NULL) {
            printf("rindow_matlib_s_imax not found.\n");
            return 0;
        }
    }
    return _g_rindow_matlib_s_imax(
        n,
        x,
        incX    
    );
}