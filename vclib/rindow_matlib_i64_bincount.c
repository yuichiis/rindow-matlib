#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_i64_bincount not found\n";
typedef void (CALLBACK* PFNrindow_matlib_i64_bincount)( /* rindow_matlib_i64_bincount */
    int32_t            /* trans */,
    int32_t            /* m */,
    int32_t            /* n */,
    int32_t            /* numClasses */,
    const int64_t *            /* input */,
    int64_t *            /* output */
);
static PFNrindow_matlib_i64_bincount _g_rindow_matlib_i64_bincount = NULL;
void rindow_matlib_i64_bincount(
    int32_t            trans,
    int32_t            m,
    int32_t            n,
    int32_t            numClasses,
    const int64_t *            input,
    int64_t *            output
)
{
    if(_g_rindow_matlib_i64_bincount==NULL) {
        _g_rindow_matlib_i64_bincount = rindow_load_rindowmatlib_func("rindow_matlib_i64_bincount"); 
        if(_g_rindow_matlib_i64_bincount==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_i64_bincount(
        trans,
        m,
        n,
        numClasses,
        input,
        output    
    );
}
