#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_tan)( /* rindow_matlib_s_tan */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_s_tan _g_rindow_matlib_s_tan = NULL;
void rindow_matlib_s_tan(
    int32_t            n,
    float *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_s_tan==NULL) {
        _g_rindow_matlib_s_tan = rindow_load_rindowmatlib_func("rindow_matlib_s_tan"); 
        if(_g_rindow_matlib_s_tan==NULL) {
            printf("rindow_matlib_s_tan not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_tan(
        n,
        x,
        incX    
    );
}
