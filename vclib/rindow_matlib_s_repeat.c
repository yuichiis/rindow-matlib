#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_repeat)( /* rindow_matlib_s_repeat */
    int32_t            /* m */,
    int32_t            /* k */,
    int32_t            /* repeats */,
    float *            /* a */,
    float *            /* b */
);
static PFNrindow_matlib_s_repeat _g_rindow_matlib_s_repeat = NULL;
void rindow_matlib_s_repeat(
    int32_t            m,
    int32_t            k,
    int32_t            repeats,
    float *            a,
    float *            b
)
{
    if(_g_rindow_matlib_s_repeat==NULL) {
        _g_rindow_matlib_s_repeat = rindow_load_rindowmatlib_func("rindow_matlib_s_repeat"); 
        if(_g_rindow_matlib_s_repeat==NULL) {
            printf("rindow_matlib_s_repeat not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_repeat(
        m,
        k,
        repeats,
        a,
        b    
    );
}
