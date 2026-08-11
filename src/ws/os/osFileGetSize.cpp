// FUNCTION_INDEX entry: osFileGetSize @0x825E5C38 (?osFileGetSize@@YAHPAUosFILE_HANDLE_DUMMY@@@Z)
#include "../../headers/ws/os/os_boundary.h"
#include "../../headers/ws/os/osFILE_HANDLE_DUMMY.h"

// Thin wrapper around Win32 GetFileSize(); passes through the INVALID_FILE_SIZE (-1) sentinel.
int osFileGetSize(osFILE_HANDLE_DUMMY *handle)
{
    unsigned int size = GetFileSize(handle, nullptr);
    if (size == (unsigned int)-1)
        return -1;
    return size;
}
