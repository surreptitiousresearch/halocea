// FUNCTION_INDEX entry: osGetCurThreadProcessor @0x825E6508 (?osGetCurThreadProcessor@@YAHXZ)
#include "../../headers/ws/os/os_boundary.h"

// Thin wrapper around Win32 GetCurrentProcessorNumber().
int osGetCurThreadProcessor()
{
    return GetCurrentProcessorNumber();
}
