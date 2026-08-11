/* file_write_to_position @0x837E6EF0 — seeks the open file to an absolute byte position (Win32 HANDLE at
 * file->data+264, see file_get_position.c); on a seek failure, clears the last error and bails out without
 * writing. Otherwise writes `count` bytes from `buffer`. */

#include <stdint.h>
#include "headers/file_reference.h"

extern unsigned int SetFilePointer(void *file, int distance, int *distance_high, unsigned int method);
extern unsigned int GetLastError(void);
extern void SetLastError(unsigned int code);
extern uint8_t file_write(const file_reference *file, unsigned int count, const void *buffer);

uint8_t file_write_to_position(const file_reference *file, unsigned int position, unsigned int count, const void *buffer)
{
    unsigned int result = SetFilePointer(file->win32.handle, position, nullptr, 0u);
    if ( result == (unsigned int)-1 )
    {
        GetLastError();
        SetLastError(0);
        return 0;
    }

    return file_write(file, count, buffer) != 0;
}
