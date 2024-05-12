#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_exp not found\n";
typedef void (CALLBACK* PFNrindow_matlib_s_exp)( /* rindow_matlib_s_exp */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */
);
static PFNrindow_matlib_s_exp _g_rindow_matlib_s_exp = NULL;
void rindow_matlib_s_exp(
    int32_t            n,
    float *            x,
    int32_t            incX
)
{
    if(_g_rindow_matlib_s_exp==NULL) {
        _g_rindow_matlib_s_exp = rindow_load_rindowmatlib_func("rindow_matlib_s_exp"); 
        if(_g_rindow_matlib_s_exp==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_s_exp(
        n,
        x,
        incX    
    );
}
