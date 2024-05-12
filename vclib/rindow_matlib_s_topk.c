#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_topk)( /* rindow_matlib_s_topk */
    int32_t            /* m */,
    int32_t            /* n */,
    const float *            /* input */,
    int32_t            /* k */,
    int32_t            /* sorted */,
    float *            /* values */,
    int32_t *            /* indices */
);
static PFNrindow_matlib_s_topk _g_rindow_matlib_s_topk = NULL;
void rindow_matlib_s_topk(
    int32_t            m,
    int32_t            n,
    const float *            input,
    int32_t            k,
    int32_t            sorted,
    float *            values,
    int32_t *            indices
)
{
    if(_g_rindow_matlib_s_topk==NULL) {
        _g_rindow_matlib_s_topk = rindow_load_rindowmatlib_func("rindow_matlib_s_topk"); 
        if(_g_rindow_matlib_s_topk==NULL) {
            printf("rindow_matlib_s_topk not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_topk(
        m,
        n,
        input,
        k,
        sorted,
        values,
        indices    
    );
}
