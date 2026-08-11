// FUNCTION_INDEX entry: osFileWrite @0x825E6BA0 (?osFileWrite@@YAHPAUosFILE_HANDLE_DUMMY@@PBXH@Z)
#include "../../headers/ws/os/os_boundary.h"
#include "../../headers/ws/os/osFILE_HANDLE_DUMMY.h"

// Thin wrapper around Win32 WriteFile(); returns -1 on failure, else the byte count written.
int osFileWrite(osFILE_HANDLE_DUMMY *handle, const void *buffer, int count)
{
    unsigned int bytesWritten;
    bool succeeded = WriteFile(handle, buffer, count, &bytesWritten, nullptr) != 0;
    if (!succeeded)
        return -1;
    return bytesWritten;
}
