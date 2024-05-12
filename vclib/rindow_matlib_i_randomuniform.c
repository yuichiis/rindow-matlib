#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_i_randomuniform)( /* rindow_matlib_i_randomuniform */
    int32_t            /* n */,
    int32_t            /* dtype */,
    void *            /* x */,
    int32_t            /* incX */,
    int32_t            /* low */,
    int32_t            /* high */,
    int32_t            /* seed */
);
static PFNrindow_matlib_i_randomuniform _g_rindow_matlib_i_randomuniform = NULL;
void rindow_matlib_i_randomuniform(
    int32_t            n,
    int32_t            dtype,
    void *            x,
    int32_t            incX,
    int32_t            low,
    int32_t            high,
    int32_t            seed
)
{
    if(_g_rindow_matlib_i_randomuniform==NULL) {
        _g_rindow_matlib_i_randomuniform = rindow_load_rindowmatlib_func("rindow_matlib_i_randomuniform"); 
        if(_g_rindow_matlib_i_randomuniform==NULL) {
            printf("rindow_matlib_i_randomuniform not found.\n");
            return;
        }
    }
    _g_rindow_matlib_i_randomuniform(
        n,
        dtype,
        x,
        incX,
        low,
        high,
        seed    
    );
}
