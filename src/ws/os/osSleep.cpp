// FUNCTION_INDEX entry: osSleep @0x825E58A8 (?osSleep@@YAXH@Z)
#include "../../headers/ws/os/os_boundary.h"

// Thin wrapper around Win32 Sleep().
void osSleep(int ms)
{
    Sleep(ms);
}
