#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_bandpart)( /* rindow_matlib_s_bandpart */
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* k */,
    float *            /* a */,
    int32_t            /* lower */,
    int32_t            /* upper */
);
static PFNrindow_matlib_s_bandpart _g_rindow_matlib_s_bandpart = NULL;
void rindow_matlib_s_bandpart(
    int32_t            m,
    int32_t            n,
    int32_t            k,
    float *            a,
    int32_t            lower,
    int32_t            upper
)
{
    if(_g_rindow_matlib_s_bandpart==NULL) {
        _g_rindow_matlib_s_bandpart = rindow_load_rindowmatlib_func("rindow_matlib_s_bandpart"); 
        if(_g_rindow_matlib_s_bandpart==NULL) {
            printf("rindow_matlib_s_bandpart not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_bandpart(
        m,
        n,
        k,
        a,
        lower,
        upper    
    );
}
