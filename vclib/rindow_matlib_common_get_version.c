#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef char* (CALLBACK* PFNrindow_matlib_common_get_version)( /* rindow_matlib_common_get_version */
    void            /*  */
);
static PFNrindow_matlib_common_get_version _g_rindow_matlib_common_get_version = NULL;
char* rindow_matlib_common_get_version(
    void            
)
{
    if(_g_rindow_matlib_common_get_version==NULL) {
        _g_rindow_matlib_common_get_version = rindow_load_rindowmatlib_func("rindow_matlib_common_get_version"); 
        if(_g_rindow_matlib_common_get_version==NULL) {
            printf("rindow_matlib_common_get_version not found.\n");
            return 0;
        }
    }
    return _g_rindow_matlib_common_get_version(
    
    );
}
