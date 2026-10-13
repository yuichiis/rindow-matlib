#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_s_omatcopy not found\n";
typedef void (CALLBACK* PFNrindow_matlib_s_omatcopy)( /* rindow_matlib_s_omatcopy */
    int32_t            /* order */,
    int32_t            /* trans */,
    int32_t            /* m */,
    int32_t            /* n */,
    float            /* alpha */,
    const float *            /* a */,
    int32_t            /* ldA */,
    float *            /* b */,
    int32_t            /* ldB */
);
static PFNrindow_matlib_s_omatcopy _g_rindow_matlib_s_omatcopy = NULL;
void rindow_matlib_s_omatcopy(
    int32_t            order,
    int32_t            trans,
    int32_t            m,
    int32_t            n,
    float            alpha,
    const float *            a,
    int32_t            ldA,
    float *            b,
    int32_t            ldB
)
{
    if(_g_rindow_matlib_s_omatcopy==NULL) {
        _g_rindow_matlib_s_omatcopy = rindow_load_rindowmatlib_func("rindow_matlib_s_omatcopy"); 
        if(_g_rindow_matlib_s_omatcopy==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_s_omatcopy(
        order,
        trans,
        m,
        n,
        alpha,
        a,
        ldA,
        b,
        ldB    
    );
}
