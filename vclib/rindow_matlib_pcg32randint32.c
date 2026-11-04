#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_pcg32randint32 not found\n";
typedef int32_t (CALLBACK* PFNrindow_matlib_pcg32randint32)( /* rindow_matlib_pcg32randint32 */
    int64_t *            /* rng */,
    int32_t            /* min */,
    int32_t            /* max */
);
static PFNrindow_matlib_pcg32randint32 _g_rindow_matlib_pcg32randint32 = NULL;
int32_t rindow_matlib_pcg32randint32(
    int64_t *            rng,
    int32_t            min,
    int32_t            max
)
{
    if(_g_rindow_matlib_pcg32randint32==NULL) {
        _g_rindow_matlib_pcg32randint32 = rindow_load_rindowmatlib_func("rindow_matlib_pcg32randint32"); 
        if(_g_rindow_matlib_pcg32randint32==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_pcg32randint32(
        rng,
        min,
        max    
    );
}
