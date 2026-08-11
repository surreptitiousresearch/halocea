// FUNCTION_INDEX entry: osDebugBreak @0x825E6148 (?osDebugBreak@@YAXXZ)
#include "../../headers/ws/os/os_boundary.h"

// Thin wrapper around Win32 DebugBreak().
void osDebugBreak()
{
    DebugBreak();
}
