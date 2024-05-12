#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_fill)( /* rindow_matlib_fill */
    int32_t            /* dtype */,
    int32_t            /* n */,
    void *            /* value */,
    void *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_fill _g_rindow_matlib_fill = NULL;
void rindow_matlib_fill(
    int32_t            dtype,
    int32_t            n,
    void *            value,
    void *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_fill==NULL) {
        _g_rindow_matlib_fill = rindow_load_rindowmatlib_func("rindow_matlib_fill"); 
        if(_g_rindow_matlib_fill==NULL) {
            printf("rindow_matlib_fill not found.\n");
            return;
        }
    }
    _g_rindow_matlib_fill(
        dtype,
        n,
        value,
        x,
        incX    
    );
}
