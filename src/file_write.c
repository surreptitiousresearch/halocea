/* file_write @0x837E6DE8 — write `count` bytes from buffer to the file_reference's open handle; succeed only
 * if all bytes were written. Clears any pending GetLastError on failure. */
#include <stdint.h>
#include <windows.h>
#include "headers/file_reference.h"
uint8_t file_write(const file_reference *file, unsigned int count, const void *buffer)
{
    uint8_t ok = 0;
    unsigned int written;
    if ( WriteFile(file->win32.handle, buffer, count, &written, 0) )
        ok = (written == count);
    if ( !ok )
    {
        GetLastError();
        SetLastError(0);
    }
    return ok;
}
