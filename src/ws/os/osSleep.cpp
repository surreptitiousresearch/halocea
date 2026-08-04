#include "../../headers/ws/os/os_boundary.h"

// Thin wrapper around Win32 Sleep().
void osSleep(int ms)
{
    Sleep(ms);
}
