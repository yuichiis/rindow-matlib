#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_randomuniform)( /* rindow_matlib_d_randomuniform */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */,
    double            /* low */,
    double            /* high */,
    int32_t            /* seed */
);
static PFNrindow_matlib_d_randomuniform _g_rindow_matlib_d_randomuniform = NULL;
void rindow_matlib_d_randomuniform(
    int32_t            n,
    double *            x,
    int32_t            incX,
    double            low,
    double            high,
    int32_t            seed
)
{
    if(_g_rindow_matlib_d_randomuniform==NULL) {
        _g_rindow_matlib_d_randomuniform = rindow_load_rindowmatlib_func("rindow_matlib_d_randomuniform"); 
        if(_g_rindow_matlib_d_randomuniform==NULL) {
            printf("rindow_matlib_d_randomuniform not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_randomuniform(
        n,
        x,
        incX,
        low,
        high,
        seed    
    );
}
