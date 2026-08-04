#include "../../headers/ws/os/os_boundary.h"
#include "../../headers/ws/os/osFILE_HANDLE_DUMMY.h"
#include "../../headers/ws/os/OS_FILE_SEEK.h"

// Maps the OS_FILE_SEEK origin to a Win32 SetFilePointer method and passes through.
// CAVEAT: the decompile has a `seekMethod >= 3` branch that reads an uninitialized stack slot
// as the Win32 method value; OS_FILE_SEEK (DB-verified via types_enum_values) only has 3 members
// (SET=0, CUR=1, END=2), so that branch is unreachable dead code from an over-wide switch/guard
// in the original source and is dropped here rather than reconstructing garbage.
unsigned int osFileSeek(osFILE_HANDLE_DUMMY *handle, int offset, OS_FILE_SEEK seekMethod)
{
    unsigned int win32Method;
    switch (seekMethod)
    {
        case OS_FILE_SEEK_SET: win32Method = 0; break; // FILE_BEGIN
        case OS_FILE_SEEK_CUR: win32Method = 1; break; // FILE_CURRENT
        default:               win32Method = 2; break; // FILE_END
    }

    unsigned int newPosition = SetFilePointer(handle, offset, nullptr, win32Method);
    if (newPosition == (unsigned int)-1)
        return (unsigned int)-1;
    return newPosition;
}
