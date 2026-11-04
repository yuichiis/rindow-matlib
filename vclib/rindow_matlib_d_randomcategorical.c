#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_d_randomcategorical not found\n";
typedef void (CALLBACK* PFNrindow_matlib_d_randomcategorical)( /* rindow_matlib_d_randomcategorical */
    int32_t            /* batchSize */,
    int32_t            /* numClasses */,
    int32_t            /* numSamples */,
    const double *            /* logits */,
    int32_t *            /* y */,
    uint64_t            /* seed */
);
static PFNrindow_matlib_d_randomcategorical _g_rindow_matlib_d_randomcategorical = NULL;
void rindow_matlib_d_randomcategorical(
    int32_t            batchSize,
    int32_t            numClasses,
    int32_t            numSamples,
    const double *            logits,
    int32_t *            y,
    uint64_t            seed
)
{
    if(_g_rindow_matlib_d_randomcategorical==NULL) {
        _g_rindow_matlib_d_randomcategorical = rindow_load_rindowmatlib_func("rindow_matlib_d_randomcategorical"); 
        if(_g_rindow_matlib_d_randomcategorical==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_d_randomcategorical(
        batchSize,
        numClasses,
        numSamples,
        logits,
        y,
        seed    
    );
}
