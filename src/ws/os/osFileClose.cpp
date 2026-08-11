// FUNCTION_INDEX entry: osFileClose @0x825E5B80 (?osFileClose@@YAXPAUosFILE_HANDLE_DUMMY@@@Z)
#include "../../headers/ws/os/os_boundary.h"
#include "../../headers/ws/os/osFILE_HANDLE_DUMMY.h"

// Thin wrapper around Win32 CloseHandle().
void osFileClose(osFILE_HANDLE_DUMMY *handle)
{
    CloseHandle(handle);
}
