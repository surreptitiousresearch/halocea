#include "../../headers/ws/os/os_boundary.h"

// Thin wrapper around the Win32 tick counter.
unsigned int osGetTime()
{
    return GetTickCount();
}
