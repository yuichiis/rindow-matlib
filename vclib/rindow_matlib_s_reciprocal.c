#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_reciprocal)( /* rindow_matlib_s_reciprocal */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */,
    float            /* alpha */,
    float            /* beta */
);
static PFNrindow_matlib_s_reciprocal _g_rindow_matlib_s_reciprocal = NULL;
void rindow_matlib_s_reciprocal(
    int32_t            n,
    float *            x,
    int32_t            incX,
    float            alpha,
    float            beta
)
{
    if(_g_rindow_matlib_s_reciprocal==NULL) {
        _g_rindow_matlib_s_reciprocal = rindow_load_rindowmatlib_func("rindow_matlib_s_reciprocal"); 
        if(_g_rindow_matlib_s_reciprocal==NULL) {
            printf("rindow_matlib_s_reciprocal not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_reciprocal(
        n,
        x,
        incX,
        alpha,
        beta    
    );
}
