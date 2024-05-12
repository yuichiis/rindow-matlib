#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_topk)( /* rindow_matlib_d_topk */
    int32_t            /* m */,
    int32_t            /* n */,
    const double *            /* input */,
    int32_t            /* k */,
    int32_t            /* sorted */,
    double *            /* values */,
    int32_t *            /* indices */
);
static PFNrindow_matlib_d_topk _g_rindow_matlib_d_topk = NULL;
void rindow_matlib_d_topk(
    int32_t            m,
    int32_t            n,
    const double *            input,
    int32_t            k,
    int32_t            sorted,
    double *            values,
    int32_t *            indices
)
{
    if(_g_rindow_matlib_d_topk==NULL) {
        _g_rindow_matlib_d_topk = rindow_load_rindowmatlib_func("rindow_matlib_d_topk"); 
        if(_g_rindow_matlib_d_topk==NULL) {
            printf("rindow_matlib_d_topk not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_topk(
        m,
        n,
        input,
        k,
        sorted,
        values,
        indices    
    );
}
