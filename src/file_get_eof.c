/* file_get_eof @0x837E6C60 — return the size in bytes of an open file. The Win32 file HANDLE lives at
 * file->data+264. On failure (INVALID_FILE_SIZE) it clears the last error and returns -1. */

#include "headers/file_reference.h"

extern unsigned int GetFileSize(void *handle, unsigned int *high);
extern unsigned int GetLastError(void);
extern void SetLastError(unsigned int code);

unsigned int file_get_eof(const file_reference *file)
{
    unsigned int size = GetFileSize(file->win32.handle, nullptr);
    if ( size == (unsigned int)-1 )
    {
        GetLastError();
        SetLastError(0);
    }
    return size;
}
