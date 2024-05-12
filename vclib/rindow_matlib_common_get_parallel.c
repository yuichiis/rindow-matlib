#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef int32_t (CALLBACK* PFNrindow_matlib_common_get_parallel)( /* rindow_matlib_common_get_parallel */
    void            /*  */
);
static PFNrindow_matlib_common_get_parallel _g_rindow_matlib_common_get_parallel = NULL;
int32_t rindow_matlib_common_get_parallel(
    void            
)
{
    if(_g_rindow_matlib_common_get_parallel==NULL) {
        _g_rindow_matlib_common_get_parallel = rindow_load_rindowmatlib_func("rindow_matlib_common_get_parallel"); 
        if(_g_rindow_matlib_common_get_parallel==NULL) {
            printf("rindow_matlib_common_get_parallel not found.\n");
            return 0;
        }
    }
    return _g_rindow_matlib_common_get_parallel(
    
    );
}
