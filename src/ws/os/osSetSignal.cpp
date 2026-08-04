#include "../../headers/ws/os/os_boundary.h"
#include "../../headers/ws/os/osHANDLE_DUMMY.h"

// Thin wrapper around Win32 SetEvent().
void osSetSignal(osHANDLE_DUMMY *handle)
{
    SetEvent(handle);
}
