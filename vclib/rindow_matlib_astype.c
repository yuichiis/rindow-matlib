#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef int32_t (CALLBACK* PFNrindow_matlib_astype)( /* rindow_matlib_astype */
    int32_t            /* n */,
    int32_t            /* from_dtype */,
    void *            /* x */,
    int32_t            /* incX */,
    int32_t            /* to_dtype */,
    void *            /* y */,
    int32_t            /* incY */
);
static PFNrindow_matlib_astype _g_rindow_matlib_astype = NULL;
int32_t rindow_matlib_astype(
    int32_t            n,
    int32_t            from_dtype,
    void *            x,
    int32_t            incX,
    int32_t            to_dtype,
    void *            y,
    int32_t            incY
)
{
    if(_g_rindow_matlib_astype==NULL) {
        _g_rindow_matlib_astype = rindow_load_rindowmatlib_func("rindow_matlib_astype"); 
        if(_g_rindow_matlib_astype==NULL) {
            printf("rindow_matlib_astype not found.\n");
            return 0;
        }
    }
    return _g_rindow_matlib_astype(
        n,
        from_dtype,
        x,
        incX,
        to_dtype,
        y,
        incY    
    );
}
