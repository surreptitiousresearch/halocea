/* file_get_size @0x837E7018 — resolve the file_reference to a full path and query its size via
 * GetFileAttributesExA. Always returns 1 (matching file_exists.c's full-path resolution pattern); *size is
 * only written on success. */

#include <stdint.h>
#include <windows.h>
#include "headers/file_reference.h"

extern void file_location_get_full_path(int16_t location, const char *path, char *full_path);

uint8_t file_get_size(const file_reference *file, unsigned int *size)
{
    char full_path[256];
    WIN32_FILE_ATTRIBUTE_DATA attr_data;
    uint8_t result = 0;

    memset(full_path, 0, sizeof(full_path));
    file_location_get_full_path(file->info.location, file->info.path, full_path);

    if ( GetFileAttributesExA(full_path, GetFileExInfoStandard, &attr_data) )
    {
        result = 1;
        *size = attr_data.nFileSizeLow;
    }

    if ( !result )
    {
        GetLastError();
        SetLastError(0);
    }

    return result;
}
