#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_d_reducesum)( /* rindow_matlib_d_reducesum */
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* k */,
    double *            /* a */,
    double *            /* b */
);
static PFNrindow_matlib_d_reducesum _g_rindow_matlib_d_reducesum = NULL;
void rindow_matlib_d_reducesum(
    int32_t            m,
    int32_t            n,
    int32_t            k,
    double *            a,
    double *            b
)
{
    if(_g_rindow_matlib_d_reducesum==NULL) {
        _g_rindow_matlib_d_reducesum = rindow_load_rindowmatlib_func("rindow_matlib_d_reducesum"); 
        if(_g_rindow_matlib_d_reducesum==NULL) {
            printf("rindow_matlib_d_reducesum not found.\n");
            return;
        }
    }
    _g_rindow_matlib_d_reducesum(
        m,
        n,
        k,
        a,
        b    
    );
}
