#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_where not found\n";
typedef void (CALLBACK* PFNrindow_matlib_d_where)( /* rindow_matlib_d_where */
    int32_t            /* trans */,
    int32_t            /* m */,
    int32_t            /* n */,
    const uint8_t *            /* cond */,
    int32_t            /* kindC */,
    const double *            /* x */,
    int32_t            /* kindX */,
    const double *            /* y */,
    int32_t            /* kindY */,
    double *            /* out */
);
static PFNrindow_matlib_d_where _g_rindow_matlib_d_where = NULL;
void rindow_matlib_d_where(
    int32_t            trans,
    int32_t            m,
    int32_t            n,
    const uint8_t *            cond,
    int32_t            kindC,
    const double *            x,
    int32_t            kindX,
    const double *            y,
    int32_t            kindY,
    double *            out
)
{
    if(_g_rindow_matlib_d_where==NULL) {
        _g_rindow_matlib_d_where = rindow_load_rindowmatlib_func("rindow_matlib_d_where"); 
        if(_g_rindow_matlib_d_where==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_d_where(
        trans,
        m,
        n,
        cond,
        kindC,
        x,
        kindX,
        y,
        kindY,
        out    
    );
}
