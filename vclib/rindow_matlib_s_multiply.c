#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_multiply)( /* rindow_matlib_s_multiply */
    int32_t            /* trans */,
    int32_t            /* m */,
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */,
    float *            /* a */,
    int32_t            /* ldA */
);
static PFNrindow_matlib_s_multiply _g_rindow_matlib_s_multiply = NULL;
void rindow_matlib_s_multiply(
    int32_t            trans,
    int32_t            m,
    int32_t            n,
    float *            x,
    int32_t            incX,
    float *            a,
    int32_t            ldA
)
{
    if(_g_rindow_matlib_s_multiply==NULL) {
        _g_rindow_matlib_s_multiply = rindow_load_rindowmatlib_func("rindow_matlib_s_multiply"); 
        if(_g_rindow_matlib_s_multiply==NULL) {
            printf("rindow_matlib_s_multiply not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_multiply(
        trans,
        m,
        n,
        x,
        incX,
        a,
        ldA    
    );
}
