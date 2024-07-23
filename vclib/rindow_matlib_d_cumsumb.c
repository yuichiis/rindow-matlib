#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_cumsumb not found\n";
typedef void (CALLBACK* PFNrindow_matlib_d_cumsumb)( /* rindow_matlib_d_cumsumb */
    int32_t            /* m */,
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */,
    int32_t            /* exclusive */,
    int32_t            /* reverse */,
    double *            /* y */,
    int32_t            /* incY */
);
static PFNrindow_matlib_d_cumsumb _g_rindow_matlib_d_cumsumb = NULL;
void rindow_matlib_d_cumsumb(
    int32_t            m,
    int32_t            n,
    double *            x,
    int32_t            incX,
    int32_t            exclusive,
    int32_t            reverse,
    double *            y,
    int32_t            incY
)
{
    if(_g_rindow_matlib_d_cumsumb==NULL) {
        _g_rindow_matlib_d_cumsumb = rindow_load_rindowmatlib_func("rindow_matlib_d_cumsumb"); 
        if(_g_rindow_matlib_d_cumsumb==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_d_cumsumb(
        m,
        n,
        x,
        incX,
        exclusive,
        reverse,
        y,
        incY    
    );
}
