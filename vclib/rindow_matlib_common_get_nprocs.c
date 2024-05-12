#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef int32_t (CALLBACK* PFNrindow_matlib_common_get_nprocs)( /* rindow_matlib_common_get_nprocs */
    void            /*  */
);
static PFNrindow_matlib_common_get_nprocs _g_rindow_matlib_common_get_nprocs = NULL;
int32_t rindow_matlib_common_get_nprocs(
    void            
)
{
    if(_g_rindow_matlib_common_get_nprocs==NULL) {
        _g_rindow_matlib_common_get_nprocs = rindow_load_rindowmatlib_func("rindow_matlib_common_get_nprocs"); 
        if(_g_rindow_matlib_common_get_nprocs==NULL) {
            printf("rindow_matlib_common_get_nprocs not found.\n");
            return 0;
        }
    }
    return _g_rindow_matlib_common_get_nprocs(
    
    );
}
