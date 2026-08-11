// FUNCTION_INDEX entry: osGetCurThreadId @0x825E6288 (?osGetCurThreadId@@YAHXZ)
#include "../../headers/ws/os/os_boundary.h"

// Thin wrapper around Win32 GetCurrentThreadId(). Previously only a boundary stand-in
// declaration existed in os_boundary.h (used by vidLOCK); that stand-in has been removed
// now that this real body exists.
int osGetCurThreadId()
{
    return GetCurrentThreadId();
}
