#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
static char msg_function_not_found[]  = "rindow_matlib_pcg32srand not found\n";
typedef void (CALLBACK* PFNrindow_matlib_pcg32srand)( /* rindow_matlib_pcg32srand */
    int64_t *            /* rng */,
    int64_t            /* seed */,
    int64_t            /* sequence */
);
static PFNrindow_matlib_pcg32srand _g_rindow_matlib_pcg32srand = NULL;
void rindow_matlib_pcg32srand(
    int64_t *            rng,
    int64_t            seed,
    int64_t            sequence
)
{
    if(_g_rindow_matlib_pcg32srand==NULL) {
        _g_rindow_matlib_pcg32srand = rindow_load_rindowmatlib_func("rindow_matlib_pcg32srand"); 
        if(_g_rindow_matlib_pcg32srand==NULL) {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);
            return;
        }
    }
    _g_rindow_matlib_pcg32srand(
        rng,
        seed,
        sequence    
    );
}
