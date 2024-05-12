#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef int32_t (CALLBACK* PFNrindow_matlib_s_gather)( /* rindow_matlib_s_gather */
    int32_t            /* reverse */,
    int32_t            /* addMode */,
    int32_t            /* n */,
    int32_t            /* k */,
    int32_t            /* numClass */,
    int32_t            /* dtype */,
    void *            /* x */,
    float *            /* a */,
    float *            /* b */
);
static PFNrindow_matlib_s_gather _g_rindow_matlib_s_gather = NULL;
int32_t rindow_matlib_s_gather(
    int32_t            reverse,
    int32_t            addMode,
    int32_t            n,
    int32_t            k,
    int32_t            numClass,
    int32_t            dtype,
    void *            x,
    float *            a,
    float *            b
)
{
    if(_g_rindow_matlib_s_gather==NULL) {
        _g_rindow_matlib_s_gather = rindow_load_rindowmatlib_func("rindow_matlib_s_gather"); 
        if(_g_rindow_matlib_s_gather==NULL) {
            printf("rindow_matlib_s_gather not found.\n");
            return 0;
        }
    }
    return _g_rindow_matlib_s_gather(
        reverse,
        addMode,
        n,
        k,
        numClass,
        dtype,
        x,
        a,
        b    
    );
}
