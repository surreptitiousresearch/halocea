// FUNCTION_INDEX entry: _osOutputDebugString @0x825E5750 (?_osOutputDebugString@@YAXPBD@Z)
#include "../../headers/ws/os/os_boundary.h"

// Thin wrapper around Win32 OutputDebugStringA().
void _osOutputDebugString(const char *str)
{
    OutputDebugStringA(str);
}
