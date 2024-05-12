#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef int32_t (CALLBACK* PFNrindow_matlib_d_transpose)( /* rindow_matlib_d_transpose */
    int32_t            /* ndim */,
    int32_t *            /* shape */,
    int32_t *            /* perm */,
    double *            /* a */,
    double *            /* b */
);
static PFNrindow_matlib_d_transpose _g_rindow_matlib_d_transpose = NULL;
int32_t rindow_matlib_d_transpose(
    int32_t            ndim,
    int32_t *            shape,
    int32_t *            perm,
    double *            a,
    double *            b
)
{
    if(_g_rindow_matlib_d_transpose==NULL) {
        _g_rindow_matlib_d_transpose = rindow_load_rindowmatlib_func("rindow_matlib_d_transpose"); 
        if(_g_rindow_matlib_d_transpose==NULL) {
            printf("rindow_matlib_d_transpose not found.\n");
            return 0;
        }
    }
    return _g_rindow_matlib_d_transpose(
        ndim,
        shape,
        perm,
        a,
        b    
    );
}
