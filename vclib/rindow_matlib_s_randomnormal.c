#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_s_randomnormal)( /* rindow_matlib_s_randomnormal */
    int32_t            /* n */,
    float *            /* x */,
    int32_t            /* incX */,
    float            /* mean */,
    float            /* scale */,
    int32_t            /* seed */
);
static PFNrindow_matlib_s_randomnormal _g_rindow_matlib_s_randomnormal = NULL;
void rindow_matlib_s_randomnormal(
    int32_t            n,
    float *            x,
    int32_t            incX,
    float            mean,
    float            scale,
    int32_t            seed
)
{
    if(_g_rindow_matlib_s_randomnormal==NULL) {
        _g_rindow_matlib_s_randomnormal = rindow_load_rindowmatlib_func("rindow_matlib_s_randomnormal"); 
        if(_g_rindow_matlib_s_randomnormal==NULL) {
            printf("rindow_matlib_s_randomnormal not found.\n");
            return;
        }
    }
    _g_rindow_matlib_s_randomnormal(
        n,
        x,
        incX,
        mean,
        scale,
        seed    
    );
}
