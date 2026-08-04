#include <stdint.h>
#include <windows.h>
#include "headers/file_reference.h"

extern void file_location_get_full_path(int16_t location, const char *path, char *full_path);

int file_read_only(const file_reference *file)
{
    char full_path[256];

    file_location_get_full_path(*(unsigned short *)&file->data[6], &file->data[8], full_path);

    DWORD attributes = GetFileAttributesA(full_path);
    if ( attributes == (DWORD)-1 )
        return 0;

    return (attributes & FILE_ATTRIBUTE_READONLY) != 0;
}
