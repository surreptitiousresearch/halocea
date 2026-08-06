/* file_set_eof @0x837E6CD4 — seek an open file to `position` and truncate it there (SetEndOfFile). The Win32
 * file HANDLE lives at file->data+264 (see file_get_position.c). Returns 1 on success, 0 otherwise; on
 * failure, clears the last error after querying it (matching the sibling file_* error-handling idiom). */

#include <stdint.h>
#include "headers/file_reference.h"

extern unsigned int SetFilePointer(void *file, int distance, int *distance_high, unsigned int method);
extern int SetEndOfFile(void *handle);
extern unsigned int GetLastError(void);
extern void SetLastError(unsigned int code);

uint8_t file_set_eof(const file_reference *file, unsigned int position)
{
    uint8_t success = 1;
    unsigned int new_position = SetFilePointer(*(void **)&file->data[264], position, nullptr, 0);

    if ( new_position == (unsigned int)-1 )
    {
        GetLastError();
        SetLastError(0);
        success = 0;
    }
    else if ( !SetEndOfFile(*(void **)&file->data[264]) )
    {
        success = 0;
    }

    if ( !success )
    {
        GetLastError();
        SetLastError(0);
    }

    return success;
}
