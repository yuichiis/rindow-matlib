#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_square)( /* rindow_matlib_d_square */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_square _g_rindow_matlib_d_square = NULL;
void rindow_matlib_d_square(
    int32_t            n,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_square==NULL) {
        _g_rindow_matlib_d_square = rindow_load_rindowmatlib_func("rindow_matlib_d_square"); 
        if(_g_rindow_matlib_d_square==NULL) {
            printf("rindow_matlib_d_square not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_square(
        n,
        x,
        incX    
    );
}
