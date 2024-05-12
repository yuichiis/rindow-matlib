#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_searchsorted not found\n";
typedef void (CALLBACK* PFNrindow_matlib_d_searchsorted)( /* rindow_matlib_d_searchsorted */
    int32_t            /* m */,
    int32_t            /* n */,
    double *            /* a */,
    int32_t            /* ldA */,
    double *            /* x */,
    int32_t            /* incX */,
    int32_t            /* right */,
    int32_t            /* dtype */,
    void *            /* y */,
    int32_t            /* incY */
);
static PFNrindow_matlib_d_searchsorted _g_rindow_matlib_d_searchsorted = NULL;
void rindow_matlib_d_searchsorted(
    int32_t            m,
    int32_t            n,
    double *            a,
    int32_t            ldA,
    double *            x,
    int32_t            incX,
    int32_t            right,
    int32_t            dtype,
    void *            y,
    int32_t            incY
)
{
    if(_g_rindow_matlib_d_searchsorted==NULL) {
        _g_rindow_matlib_d_searchsorted = rindow_load_rindowmatlib_func("rindow_matlib_d_searchsorted"); 
        if(_g_rindow_matlib_d_searchsorted==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_d_searchsorted(
        m,
        n,
        a,
        ldA,
        x,
        incX,
        right,
        dtype,
        y,
        incY    
    );
}
