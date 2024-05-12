#include <stdio.h>
#include <Windows.h>
#include <rindow/matlib.h>
#include "vclib.h"

static HMODULE _h_rindowmatlib = NULL;
void* rindow_load_rindowmatlib_func(char *funcname)
{
    if(_h_rindowmatlib==NULL) {
        _h_rindowmatlib = LoadLibraryA( "rindowmatlib.dll" );
        if(_h_rindowmatlib==NULL) {
            printf("Load error: rindowmatlib\n");
            return NULL;
        }
    }
    return GetProcAddress( _h_rindowmatlib, funcname );
}
