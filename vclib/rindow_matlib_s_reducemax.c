#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_reducemax)( /* rindow_matlib_s_reducemax */
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* k */,
    float *            /* a */,
    float *            /* b */
);
static PFNrindow_matlib_s_reducemax _g_rindow_matlib_s_reducemax = NULL;
void rindow_matlib_s_reducemax(
    int32_t            m,
    int32_t            n,
    int32_t            k,
    float *            a,
    float *            b
)
{
    if(_g_rindow_matlib_s_reducemax==NULL) {
        _g_rindow_matlib_s_reducemax = rindow_load_rindowmatlib_func("rindow_matlib_s_reducemax"); 
        if(_g_rindow_matlib_s_reducemax==NULL) {
            printf("rindow_matlib_s_reducemax not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_reducemax(
        m,
        n,
        k,
        a,
        b    
    );
}
