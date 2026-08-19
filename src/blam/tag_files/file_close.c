/* file_close @0x837E6B38 — close the file_reference's open Win32 handle (at data[264]); clear it and report
 * success. Clears any pending GetLastError on failure. */
#include <stdint.h>
#include <windows.h>
#include "headers/file_reference.h"
uint8_t file_close(file_reference *file)
{
    uint8_t closed = 0;
    if ( CloseHandle(file->win32.handle) )
    {
        closed = 1;
        file->win32.handle = 0;
    }
    if ( !closed )
    {
        GetLastError();
        SetLastError(0);
    }
    return closed;
}
