#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_increment)( /* rindow_matlib_s_increment */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */,
    float            /* alpha */,
    float            /* beta */
);
static PFNrindow_matlib_s_increment _g_rindow_matlib_s_increment = NULL;
void rindow_matlib_s_increment(
    int32_t            n,
    float *            x,
    int32_t            incX,
    float            alpha,
    float            beta
)
{
    if(_g_rindow_matlib_s_increment==NULL) {
        _g_rindow_matlib_s_increment = rindow_load_rindowmatlib_func("rindow_matlib_s_increment"); 
        if(_g_rindow_matlib_s_increment==NULL) {
            printf("rindow_matlib_s_increment not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_increment(
        n,
        x,
        incX,
        alpha,
        beta    
    );
}
