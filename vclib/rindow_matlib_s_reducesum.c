#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_reducesum)( /* rindow_matlib_s_reducesum */
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* k */,
    float *            /* a */,
    float *            /* b */
);
static PFNrindow_matlib_s_reducesum _g_rindow_matlib_s_reducesum = NULL;
void rindow_matlib_s_reducesum(
    int32_t            m,
    int32_t            n,
    int32_t            k,
    float *            a,
    float *            b
)
{
    if(_g_rindow_matlib_s_reducesum==NULL) {
        _g_rindow_matlib_s_reducesum = rindow_load_rindowmatlib_func("rindow_matlib_s_reducesum"); 
        if(_g_rindow_matlib_s_reducesum==NULL) {
            printf("rindow_matlib_s_reducesum not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_reducesum(
        m,
        n,
        k,
        a,
        b    
    );
}
