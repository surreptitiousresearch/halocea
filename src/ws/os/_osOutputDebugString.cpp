#include "../../headers/ws/os/os_boundary.h"

// Thin wrapper around Win32 OutputDebugStringA().
void _osOutputDebugString(const char *str)
{
    OutputDebugStringA(str);
}
