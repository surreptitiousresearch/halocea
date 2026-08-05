/* file_create @0x837E66E8 — create the file (or directory) named by a file_reference. For a directory
 * reference (name flag clear) it calls CreateDirectory; for a file it resolves the full path, truncates the
 * leaf to 40 chars if over-long, and creates (CREATE_ALWAYS) then immediately closes the handle. Returns 1 on
 * success. Clears any pending GetLastError on failure. */

#include <stdint.h>
#include <windows.h>
#include "headers/file_reference.h"

extern void *memset(void *dst, int c, size_t n);
extern void file_location_get_full_path(int16_t location, const char *path, char *full_path);
extern const char *tag_name_strip_path(const char *name);

uint8_t file_create(file_reference *file)
{
    int created = 0;
    char full_path[288];
    memset(full_path, 0, 256);
    file_location_get_full_path(*(unsigned short *)&file->data[6], &file->data[8], full_path);

    if ( (*(unsigned short *)&file->data[4] & 1) == 0 )   /* name-set flag word at data[4] (file_reference is an opaque char[268] in the DB) */
    {
        if ( CreateDirectoryA((const char *)&file->data[8], 0) )
            created = 1;
    }
    else
    {
        const char *leaf = tag_name_strip_path(full_path);
        const char *end = leaf;
        while ( *end++ )
            ;
        if ( (unsigned int)(end - leaf - 1) > 0x28 )
        {
            char *leaf_mutable = (char *)leaf;
            leaf_mutable[40] = 0;   /* truncate over-long leaf to 40 chars */
        }

        /* GENERIC_WRITE, no share, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL */
        void *handle = CreateFileA(full_path, 0x40000000u, 0, 0, 2u, 0x80u, 0);
        if ( handle != (void *)-1 )
        {
            CloseHandle(handle);
            created = 1;
        }
    }

    if ( !(unsigned char)created )
    {
        GetLastError();
        SetLastError(0);
    }
    return created;
}
