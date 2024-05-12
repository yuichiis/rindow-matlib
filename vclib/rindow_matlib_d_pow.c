#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_pow)( /* rindow_matlib_d_pow */
    int32_t            /* trans */,
    int32_t            /* m */,
    int32_t            /* n */,
    double *            /* a */,
    int32_t            /* ldA */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_pow _g_rindow_matlib_d_pow = NULL;
void rindow_matlib_d_pow(
    int32_t            trans,
    int32_t            m,
    int32_t            n,
    double *            a,
    int32_t            ldA,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_pow==NULL) {
        _g_rindow_matlib_d_pow = rindow_load_rindowmatlib_func("rindow_matlib_d_pow"); 
        if(_g_rindow_matlib_d_pow==NULL) {
            printf("rindow_matlib_d_pow not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_pow(
        trans,
        m,
        n,
        a,
        ldA,
        x,
        incX    
    );
}
