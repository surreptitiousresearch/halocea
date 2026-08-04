#include "../../headers/ws/os/os_boundary.h"
#include "../../headers/ws/os/osFILE_HANDLE_DUMMY.h"

// Thin wrapper around Win32 CloseHandle().
void osFileClose(osFILE_HANDLE_DUMMY *handle)
{
    CloseHandle(handle);
}
