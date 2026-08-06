/* file_get_position @0x837E6BA8 — current seek offset of an open file. The Win32 file HANDLE lives at
 * file->data+264. On failure (INVALID_SET_FILE_POINTER) it clears the last error. */

#include "headers/file_reference.h"

extern unsigned int SetFilePointer(void *file, int distance, int *distance_high, unsigned int method);
extern unsigned int GetLastError(void);
extern void SetLastError(unsigned int code);

unsigned int file_get_position(const file_reference *file)
{
    unsigned int position = SetFilePointer(*(void **)&file->data[264], 0, nullptr, 1u);
    if (position == (unsigned int)-1)
    {
        GetLastError();
        SetLastError(0);
    }
    return position;
}
