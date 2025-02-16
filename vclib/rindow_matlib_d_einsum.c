#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_einsum not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_d_einsum)( /* rindow_matlib_d_einsum */
    int32_t            /* depth */,
    int32_t *            /* sizeOfIndices */,
    double *            /* a */,
    int32_t            /* ndimA */,
    int32_t *            /* labelA */,
    double *            /* b */,
    int32_t            /* ndimB */,
    int32_t *            /* labelB */,
    double *            /* c */,
    int32_t            /* ndimC */,
    int32_t *            /* shapeA */,
    int32_t *            /* shapeB */
);
static PFNrindow_matlib_d_einsum _g_rindow_matlib_d_einsum = NULL;
int32_t rindow_matlib_d_einsum(
    int32_t            depth,
    int32_t *            sizeOfIndices,
    double *            a,
    int32_t            ndimA,
    int32_t *            labelA,
    double *            b,
    int32_t            ndimB,
    int32_t *            labelB,
    double *            c,
    int32_t            ndimC,
    int32_t *            shapeA,
    int32_t *            shapeB
)
{
    if(_g_rindow_matlib_d_einsum==NULL) {
        _g_rindow_matlib_d_einsum = rindow_load_rindowmatlib_func("rindow_matlib_d_einsum"); 
        if(_g_rindow_matlib_d_einsum==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_d_einsum(
        depth,
        sizeOfIndices,
        a,
        ndimA,
        labelA,
        b,
        ndimB,
        labelB,
        c,
        ndimC,
        shapeA,
        shapeB    
    );
}
