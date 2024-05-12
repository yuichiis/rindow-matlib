#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_softmax not found\n";
typedef void (CALLBACK* PFNrindow_matlib_s_softmax)( /* rindow_matlib_s_softmax */
    int32_t            /* m */,
    int32_t            /* n */,
    float *            /* a */,
    int32_t            /* ldA */
);
static PFNrindow_matlib_s_softmax _g_rindow_matlib_s_softmax = NULL;
void rindow_matlib_s_softmax(
    int32_t            m,
    int32_t            n,
    float *            a,
    int32_t            ldA
)
{
    if(_g_rindow_matlib_s_softmax==NULL) {
        _g_rindow_matlib_s_softmax = rindow_load_rindowmatlib_func("rindow_matlib_s_softmax"); 
        if(_g_rindow_matlib_s_softmax==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_s_softmax(
        m,
        n,
        a,
        ldA    
    );
}
