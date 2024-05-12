#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_increment)( /* rindow_matlib_d_increment */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */,
    double            /* alpha */,
    double            /* beta */
);
static PFNrindow_matlib_d_increment _g_rindow_matlib_d_increment = NULL;
void rindow_matlib_d_increment(
    int32_t            n,
    double *            x,
    int32_t            incX,
    double            alpha,
    double            beta
)
{
    if(_g_rindow_matlib_d_increment==NULL) {
        _g_rindow_matlib_d_increment = rindow_load_rindowmatlib_func("rindow_matlib_d_increment"); 
        if(_g_rindow_matlib_d_increment==NULL) {
            printf("rindow_matlib_d_increment not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_increment(
        n,
        x,
        incX,
        alpha,
        beta    
    );
}
