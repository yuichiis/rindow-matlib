#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_z_omatcopy not found\n";
typedef void (CALLBACK* PFNrindow_matlib_z_omatcopy)( /* rindow_matlib_z_omatcopy */
    int32_t            /* order */,
    int32_t            /* trans */,
    int32_t            /* m */,
    int32_t            /* n */,
    const void *            /* alpha */,
    const void *            /* a */,
    int32_t            /* ldA */,
    void *            /* b */,
    int32_t            /* ldB */
);
static PFNrindow_matlib_z_omatcopy _g_rindow_matlib_z_omatcopy = NULL;
void rindow_matlib_z_omatcopy(
    int32_t            order,
    int32_t            trans,
    int32_t            m,
    int32_t            n,
    const void *            alpha,
    const void *            a,
    int32_t            ldA,
    void *            b,
    int32_t            ldB
)
{
    if(_g_rindow_matlib_z_omatcopy==NULL) {
        _g_rindow_matlib_z_omatcopy = rindow_load_rindowmatlib_func("rindow_matlib_z_omatcopy"); 
        if(_g_rindow_matlib_z_omatcopy==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_z_omatcopy(
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
