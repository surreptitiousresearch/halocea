#include "../../headers/ws/os/os_boundary.h"
#include "../../headers/ws/os/osHANDLE_DUMMY.h"

// Thin wrapper around Win32 ResetEvent().
void osResetSignal(osHANDLE_DUMMY *handle)
{
    ResetEvent(handle);
}
