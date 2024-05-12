#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_searchsorted)( /* rindow_matlib_s_searchsorted */
    int32_t            /* m */,
    int32_t            /* n */,
    float *            /* a */,
    int32_t            /* ldA */,
    float *            /* x */,
    int32_t            /* incX */,
    int32_t            /* right */,
    int32_t            /* dtype */,
    void *            /* y */,
    int32_t            /* incY */
);
static PFNrindow_matlib_s_searchsorted _g_rindow_matlib_s_searchsorted = NULL;
void rindow_matlib_s_searchsorted(
    int32_t            m,
    int32_t            n,
    float *            a,
    int32_t            ldA,
    float *            x,
    int32_t            incX,
    int32_t            right,
    int32_t            dtype,
    void *            y,
    int32_t            incY
)
{
    if(_g_rindow_matlib_s_searchsorted==NULL) {
        _g_rindow_matlib_s_searchsorted = rindow_load_rindowmatlib_func("rindow_matlib_s_searchsorted"); 
        if(_g_rindow_matlib_s_searchsorted==NULL) {
            printf("rindow_matlib_s_searchsorted not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_searchsorted(
        m,
        n,
        a,
        ldA,
        x,
        incX,
        right,
        dtype,
        y,
        incY    
    );
}
