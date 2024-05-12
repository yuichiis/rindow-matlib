#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_add)( /* rindow_matlib_d_add */
    int32_t            /* trans */,
    int32_t            /* m */,
    int32_t            /* n */,
    double            /* alpha */,
    double *            /* x */,
    int32_t            /* incX */,
    double *            /* a */,
    int32_t            /* ldA */
);
static PFNrindow_matlib_d_add _g_rindow_matlib_d_add = NULL;
void rindow_matlib_d_add(
    int32_t            trans,
    int32_t            m,
    int32_t            n,
    double            alpha,
    double *            x,
    int32_t            incX,
    double *            a,
    int32_t            ldA
)
{
    if(_g_rindow_matlib_d_add==NULL) {
        _g_rindow_matlib_d_add = rindow_load_rindowmatlib_func("rindow_matlib_d_add"); 
        if(_g_rindow_matlib_d_add==NULL) {
            printf("rindow_matlib_d_add not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_add(
        trans,
        m,
        n,
        alpha,
        x,
        incX,
        a,
        ldA    
    );
}
