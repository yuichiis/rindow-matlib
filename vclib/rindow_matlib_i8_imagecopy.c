#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"
typedef void (CALLBACK* PFNrindow_matlib_i8_imagecopy)( /* rindow_matlib_i8_imagecopy */
    int32_t            /* height */,
    int32_t            /* width */,
    int32_t            /* channels */,
    uint8_t *            /* a */,
    uint8_t *            /* b */,
    int32_t            /* channelsFirst */,
    int32_t            /* heightShift */,
    int32_t            /* widthShift */,
    int32_t            /* verticalFlip */,
    int32_t            /* horizontalFlip */,
    int32_t            /* rgbFlip */
);
static PFNrindow_matlib_i8_imagecopy _g_rindow_matlib_i8_imagecopy = NULL;
void rindow_matlib_i8_imagecopy(
    int32_t            height,
    int32_t            width,
    int32_t            channels,
    uint8_t *            a,
    uint8_t *            b,
    int32_t            channelsFirst,
    int32_t            heightShift,
    int32_t            widthShift,
    int32_t            verticalFlip,
    int32_t            horizontalFlip,
    int32_t            rgbFlip
)
{
    if(_g_rindow_matlib_i8_imagecopy==NULL) {
        _g_rindow_matlib_i8_imagecopy = rindow_load_rindowmatlib_func("rindow_matlib_i8_imagecopy"); 
        if(_g_rindow_matlib_i8_imagecopy==NULL) {
            printf("rindow_matlib_i8_imagecopy not found.\n");
            return;
        }
    }
    _g_rindow_matlib_i8_imagecopy(
        height,
        width,
        channels,
        a,
        b,
        channelsFirst,
        heightShift,
        widthShift,
        verticalFlip,
        horizontalFlip,
        rgbFlip    
    );
}
