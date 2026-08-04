#include "../../headers/ws/os/os_boundary.h"

// Thin wrapper around Win32 GetCurrentProcessorNumber().
int osGetCurThreadProcessor()
{
    return GetCurrentProcessorNumber();
}
