#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void* (CALLBACK* PFNrindow_matlib_common_get_address)( /* rindow_matlib_common_get_address */
    int32_t            /* dtype */,
    void *            /* buffer */,
    int32_t            /* offset */
);
static PFNrindow_matlib_common_get_address _g_rindow_matlib_common_get_address = NULL;
void* rindow_matlib_common_get_address(
    int32_t            dtype,
    void *            buffer,
    int32_t            offset
)
{
    if(_g_rindow_matlib_common_get_address==NULL) {
        _g_rindow_matlib_common_get_address = rindow_load_rindowmatlib_func("rindow_matlib_common_get_address"); 
        if(_g_rindow_matlib_common_get_address==NULL) {
            printf("rindow_matlib_common_get_address not found.\n");
            return 0;
        }
    }
    return _g_rindow_matlib_common_get_address(
        dtype,
        buffer,
        offset    
    );
}
