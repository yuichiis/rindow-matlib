#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef int32_t (CALLBACK* PFNrindow_matlib_common_get_num_threads)( /* rindow_matlib_common_get_num_threads */
    void            /*  */
);
static PFNrindow_matlib_common_get_num_threads _g_rindow_matlib_common_get_num_threads = NULL;
int32_t rindow_matlib_common_get_num_threads(
    void            
)
{
    if(_g_rindow_matlib_common_get_num_threads==NULL) {
        _g_rindow_matlib_common_get_num_threads = rindow_load_rindowmatlib_func("rindow_matlib_common_get_num_threads"); 
        if(_g_rindow_matlib_common_get_num_threads==NULL) {
            printf("rindow_matlib_common_get_num_threads not found.\n");
            return 0;
        }
    }
    return _g_rindow_matlib_common_get_num_threads(
    
    );
}
