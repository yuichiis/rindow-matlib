#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_duplicate)( /* rindow_matlib_d_duplicate */
    int32_t            /* trans */,
    int32_t            /* m */,
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */,
    double *            /* a */,
    int32_t            /* ldA */
);
static PFNrindow_matlib_d_duplicate _g_rindow_matlib_d_duplicate = NULL;
void rindow_matlib_d_duplicate(
    int32_t            trans,
    int32_t            m,
    int32_t            n,
    double *            x,
    int32_t            incX,
    double *            a,
    int32_t            ldA
)
{
    if(_g_rindow_matlib_d_duplicate==NULL) {
        _g_rindow_matlib_d_duplicate = rindow_load_rindowmatlib_func("rindow_matlib_d_duplicate"); 
        if(_g_rindow_matlib_d_duplicate==NULL) {
            printf("rindow_matlib_d_duplicate not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_duplicate(
        trans,
        m,
        n,
        x,
        incX,
        a,
        ldA    
    );
}
