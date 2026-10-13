#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_range not found\n";
typedef void (CALLBACK* PFNrindow_matlib_s_range)( /* rindow_matlib_s_range */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */,
    float            /* start */,
    float            /* limit */,
    float            /* delta */
);
static PFNrindow_matlib_s_range _g_rindow_matlib_s_range = NULL;
void rindow_matlib_s_range(
    int32_t            n,
    float *            x,
    int32_t            incX,
    float            start,
    float            limit,
    float            delta
)
{
    if(_g_rindow_matlib_s_range==NULL) {
        _g_rindow_matlib_s_range = rindow_load_rindowmatlib_func("rindow_matlib_s_range"); 
        if(_g_rindow_matlib_s_range==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_s_range(
        n,
        x,
        incX,
        start,
        limit,
        delta    
    );
}
