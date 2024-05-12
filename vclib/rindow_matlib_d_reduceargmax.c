#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_reduceargmax)( /* rindow_matlib_d_reduceargmax */
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* k */,
    double *            /* a */,
    int32_t            /* dtype */,
    void *            /* b */
);
static PFNrindow_matlib_d_reduceargmax _g_rindow_matlib_d_reduceargmax = NULL;
void rindow_matlib_d_reduceargmax(
    int32_t            m,
    int32_t            n,
    int32_t            k,
    double *            a,
    int32_t            dtype,
    void *            b
)
{
    if(_g_rindow_matlib_d_reduceargmax==NULL) {
        _g_rindow_matlib_d_reduceargmax = rindow_load_rindowmatlib_func("rindow_matlib_d_reduceargmax"); 
        if(_g_rindow_matlib_d_reduceargmax==NULL) {
            printf("rindow_matlib_d_reduceargmax not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_reduceargmax(
        m,
        n,
        k,
        a,
        dtype,
        b    
    );
}