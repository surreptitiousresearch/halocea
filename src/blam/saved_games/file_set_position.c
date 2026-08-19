/* file_set_position @0x837E6C00 — seek a file_reference's underlying Win32 handle to an absolute position.
 * Returns 1 on success; on failure clears the OS error and returns 0. */

#include <stdint.h>
#include "headers/file_reference.h"
#include <windows.h>

uint8_t file_set_position(const file_reference *file, unsigned int position)
{
    unsigned int result = SetFilePointer(file->win32.handle, position, 0, FILE_BEGIN);
    unsigned char succeeded = result != (unsigned int)-1;
    if ( result == (unsigned int)-1 )
    {
        GetLastError();
        SetLastError(0);
    }
    return succeeded;
}
