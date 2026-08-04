#include "../../headers/ws/os/os_boundary.h"

// Thin wrapper around Win32 DebugBreak().
void osDebugBreak()
{
    DebugBreak();
}
