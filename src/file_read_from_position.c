/* file_read_from_position @0x837E6E8C — seeks the open file to an absolute byte position (Win32 HANDLE at
 * file->data+264, see file_get_position.c); on a seek failure, clears the last error and bails out without
 * reading. Otherwise reads `count` bytes into `buffer`. */

#include <stdint.h>
#include "headers/file_reference.h"

extern unsigned int SetFilePointer(void *file, int distance, int *distance_high, unsigned int method);
extern unsigned int GetLastError(void);
extern void SetLastError(unsigned int code);
extern uint8_t file_read(const file_reference *file, unsigned int count, void *buffer);

uint8_t file_read_from_position(const file_reference *file, unsigned int position, unsigned int count, void *buffer)
{
    unsigned int result = SetFilePointer(*(void **)&file->data[264], position, nullptr, 0u);
    if ( result == (unsigned int)-1 )
    {
        GetLastError();
        SetLastError(0);
        return 0;
    }

    return file_read(file, count, buffer) != 0;
}
