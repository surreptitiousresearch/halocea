/* file_exists @0x837E6890 — resolve the file_reference to a full path and test it with GetFileAttributes.
 * Returns 1 if it exists, 0 otherwise (treating not-found / path-not-found as a clean absence). */
#include <stdint.h>
#include <windows.h>
#include "headers/file_reference.h"
extern void *memset(void *dst, int c, uint64_t n);
extern void file_location_get_full_path(int16_t location, const char *path, char *full_path);
uint8_t file_exists(const file_reference *file)
{
    char full_path[256];
    memset(full_path, 0, sizeof(full_path));
    file_location_get_full_path(*(unsigned short *)&file->data[6], &file->data[8], full_path);
    if ( GetFileAttributesA(full_path) != (DWORD)-1 )
        return 1;
    if ( GetLastError() != 2 && GetLastError() != 3 )
    {
        GetLastError();
        SetLastError(0);
    }
    return 0;
}
