#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_i_randomsequence)( /* rindow_matlib_i_randomsequence */
    int32_t            /* n */,
    int32_t            /* size */,
    int32_t            /* dtype */,
    void *            /* x */,
    int32_t            /* incX */,
    int32_t            /* seed */
);
static PFNrindow_matlib_i_randomsequence _g_rindow_matlib_i_randomsequence = NULL;
void rindow_matlib_i_randomsequence(
    int32_t            n,
    int32_t            size,
    int32_t            dtype,
    void *            x,
    int32_t            incX,
    int32_t            seed
)
{
    if(_g_rindow_matlib_i_randomsequence==NULL) {
        _g_rindow_matlib_i_randomsequence = rindow_load_rindowmatlib_func("rindow_matlib_i_randomsequence"); 
        if(_g_rindow_matlib_i_randomsequence==NULL) {
            printf("rindow_matlib_i_randomsequence not found.\n");
            return;
        }
    }
    _g_rindow_matlib_i_randomsequence(
        n,
        size,
        dtype,
        x,
        incX,
        seed    
    );
}
