#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_linspace not found\n";
typedef void (CALLBACK* PFNrindow_matlib_d_linspace)( /* rindow_matlib_d_linspace */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */,
    double            /* start */,
    double            /* stop */
);
static PFNrindow_matlib_d_linspace _g_rindow_matlib_d_linspace = NULL;
void rindow_matlib_d_linspace(
    int32_t            n,
    double *            x,
    int32_t            incX,
    double            start,
    double            stop
)
{
    if(_g_rindow_matlib_d_linspace==NULL) {
        _g_rindow_matlib_d_linspace = rindow_load_rindowmatlib_func("rindow_matlib_d_linspace"); 
        if(_g_rindow_matlib_d_linspace==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_d_linspace(
        n,
        x,
        incX,
        start,
        stop    
    );
}
