#include "../../headers/ws/os/os_boundary.h"
#include "../../headers/ws/os/osHANDLE_DUMMY.h"

// Thin wrapper around Win32 CloseHandle() -- signals are ordinary Win32 event HANDLEs.
void osDestroySignal(osHANDLE_DUMMY *handle)
{
    CloseHandle(handle);
}
