#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_softmax)( /* rindow_matlib_d_softmax */
    int32_t            /* m */,
    int32_t            /* n */,
    double *            /* a */,
    int32_t            /* ldA */
);
static PFNrindow_matlib_d_softmax _g_rindow_matlib_d_softmax = NULL;
void rindow_matlib_d_softmax(
    int32_t            m,
    int32_t            n,
    double *            a,
    int32_t            ldA
)
{
    if(_g_rindow_matlib_d_softmax==NULL) {
        _g_rindow_matlib_d_softmax = rindow_load_rindowmatlib_func("rindow_matlib_d_softmax"); 
        if(_g_rindow_matlib_d_softmax==NULL) {
            printf("rindow_matlib_d_softmax not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_softmax(
        m,
        n,
        a,
        ldA    
    );
}
