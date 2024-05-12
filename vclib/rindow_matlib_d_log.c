#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_log not found\n";
typedef void (CALLBACK* PFNrindow_matlib_d_log)( /* rindow_matlib_d_log */
    int32_t            /* n */,
    double *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_d_log _g_rindow_matlib_d_log = NULL;
void rindow_matlib_d_log(
    int32_t            n,
    double *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_d_log==NULL) {
        _g_rindow_matlib_d_log = rindow_load_rindowmatlib_func("rindow_matlib_d_log"); 
        if(_g_rindow_matlib_d_log==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_d_log(
        n,
        x,
        incX    
    );
}
