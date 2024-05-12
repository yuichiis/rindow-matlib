#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef int32_t (CALLBACK* PFNrindow_matlib_i_reducegather)( /* rindow_matlib_i_reducegather */
    int32_t            /* reverse */,
    int32_t            /* addMode */,
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* numClass */,
    int32_t            /* dtype */,
    void *            /* x */,
    int32_t            /* data_dtype */,
    void *            /* a */,
    void *            /* b */
);
static PFNrindow_matlib_i_reducegather _g_rindow_matlib_i_reducegather = NULL;
int32_t rindow_matlib_i_reducegather(
    int32_t            reverse,
    int32_t            addMode,
    int32_t            m,
    int32_t            n,
    int32_t            numClass,
    int32_t            dtype,
    void *            x,
    int32_t            data_dtype,
    void *            a,
    void *            b
)
{
    if(_g_rindow_matlib_i_reducegather==NULL) {
        _g_rindow_matlib_i_reducegather = rindow_load_rindowmatlib_func("rindow_matlib_i_reducegather"); 
        if(_g_rindow_matlib_i_reducegather==NULL) {
            printf("rindow_matlib_i_reducegather not found.\n");
            return 0;
        }
    }
    return _g_rindow_matlib_i_reducegather(
        reverse,
        addMode,
        m,
        n,
        numClass,
        dtype,
        x,
        data_dtype,
        a,
        b    
    );
}
