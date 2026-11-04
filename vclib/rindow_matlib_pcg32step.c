#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_pcg32step not found\n";
typedef void (CALLBACK* PFNrindow_matlib_pcg32step)( /* rindow_matlib_pcg32step */
    int64_t *            /* rng */
);
static PFNrindow_matlib_pcg32step _g_rindow_matlib_pcg32step = NULL;
void rindow_matlib_pcg32step(
    int64_t *            rng
)
{
    if(_g_rindow_matlib_pcg32step==NULL) {
        _g_rindow_matlib_pcg32step = rindow_load_rindowmatlib_func("rindow_matlib_pcg32step"); 
        if(_g_rindow_matlib_pcg32step==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_pcg32step(
        rng    
    );
}
