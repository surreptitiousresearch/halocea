/* file_read @0x837E6D58 — read exactly `count` bytes from a file_reference's Win32 handle into buffer. Returns
 * 1 only on a full read; a short read sets ERROR_HANDLE_EOF (0x26). On any failure the OS error is cleared. */

#include <stdint.h>
#include "headers/file_reference.h"
#include <windows.h>

uint8_t file_read(const file_reference *file, unsigned int count, void *buffer)
{
    unsigned char succeeded = 0;
    unsigned int bytes_read;

    if ( ReadFile(file->win32.handle, buffer, count, &bytes_read, 0) )
    {
        if ( bytes_read == count )
            succeeded = 1;
        else
            SetLastError(0x26u);
    }

    if ( !succeeded )
    {
        GetLastError();
        SetLastError(0);
    }
    return succeeded;
}
