#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_pow)( /* rindow_matlib_s_pow */
    int32_t            /* trans */,
    int32_t            /* m */,
    int32_t            /* n */,
    float *            /* a */,
    int32_t            /* ldA */,
    float *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_s_pow _g_rindow_matlib_s_pow = NULL;
void rindow_matlib_s_pow(
    int32_t            trans,
    int32_t            m,
    int32_t            n,
    float *            a,
    int32_t            ldA,
    float *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_s_pow==NULL) {
        _g_rindow_matlib_s_pow = rindow_load_rindowmatlib_func("rindow_matlib_s_pow"); 
        if(_g_rindow_matlib_s_pow==NULL) {
            printf("rindow_matlib_s_pow not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_pow(
        trans,
        m,
        n,
        a,
        ldA,
        x,
        incX    
    );
}
