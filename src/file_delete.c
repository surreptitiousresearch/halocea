/* file_delete @0x837E67F0 — delete the file or directory named by a file_reference. Resolves the full path
 * from the location id (data+6) and name (data+8); for a file (flag bit 0 @ data+4) it clears the
 * read-only attribute then DeleteFileA, for a directory it RemoveDirectoryA. On failure the Win32 error is
 * read and cleared. Returns 1 on success, 0 otherwise. */

#include <stdint.h>
#include "headers/file_reference.h"

extern void *memset(void *dst, int value, unsigned int count);
extern void file_location_get_full_path(int16_t location, const char *path, char *full_path);
extern int SetFileAttributesA(const char *path, unsigned int attributes);
extern int DeleteFileA(const char *path);
extern int RemoveDirectoryA(const char *path);
extern unsigned int GetLastError(void);
extern void SetLastError(unsigned int error);

int file_delete(file_reference *file)
{
    int succeeded = 0;
    char full_path[264];
    memset(full_path, 0, 256);
    file_location_get_full_path(*(__int16 *)&file->data[6], &file->data[8], full_path);

    int result;
    if ( (*(__int16 *)&file->data[4] & 1) != 0 )
    {
        if ( !SetFileAttributesA(full_path, 0x80u) )
            goto failed;
        result = DeleteFileA(full_path);
    }
    else
    {
        result = RemoveDirectoryA(full_path);
    }
    if ( result )
        succeeded = 1;

failed:
    if ( !(unsigned char)succeeded )
    {
        GetLastError();
        SetLastError(0);
    }
    return succeeded;
}
