#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef int32_t (CALLBACK* PFNrindow_matlib_s_onehot)( /* rindow_matlib_s_onehot */
    int32_t            /* dtype */,
    int32_t            /* m */,
    int32_t            /* n */,
    void *            /* x */,
    int32_t            /* incX */,
    float            /* alpha */,
    float *            /* a */,
    int32_t            /* ldA */
);
static PFNrindow_matlib_s_onehot _g_rindow_matlib_s_onehot = NULL;
int32_t rindow_matlib_s_onehot(
    int32_t            dtype,
    int32_t            m,
    int32_t            n,
    void *            x,
    int32_t            incX,
    float            alpha,
    float *            a,
    int32_t            ldA
)
{
    if(_g_rindow_matlib_s_onehot==NULL) {
        _g_rindow_matlib_s_onehot = rindow_load_rindowmatlib_func("rindow_matlib_s_onehot"); 
        if(_g_rindow_matlib_s_onehot==NULL) {
            printf("rindow_matlib_s_onehot not found.\n");
            return 0;
        }
    }
    return _g_rindow_matlib_s_onehot(
        dtype,
        m,
        n,
        x,
        incX,
        alpha,
        a,
        ldA    
    );
}
