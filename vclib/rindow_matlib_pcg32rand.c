#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_pcg32rand not found\n";
typedef uint32_t (CALLBACK* PFNrindow_matlib_pcg32rand)( /* rindow_matlib_pcg32rand */
    int64_t *            /* rng */
);
static PFNrindow_matlib_pcg32rand _g_rindow_matlib_pcg32rand = NULL;
uint32_t rindow_matlib_pcg32rand(
    int64_t *            rng
)
{
    if(_g_rindow_matlib_pcg32rand==NULL) {
        _g_rindow_matlib_pcg32rand = rindow_load_rindowmatlib_func("rindow_matlib_pcg32rand"); 
        if(_g_rindow_matlib_pcg32rand==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return 0;
        }
    }
    return _g_rindow_matlib_pcg32rand(
        rng    
    );
}
