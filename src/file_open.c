/* file_open @0x837E6A08 — open the file named by a file_reference with the given access flags
 * (1=read, 2=write, 4=seek-to-end), storing the Win32 handle at data[264]. Truncates an over-long leaf name to
 * 40 chars. Returns 1 on success; on a failed seek-to-end it closes the handle and fails. */

#include <stdint.h>
#include <windows.h>
#include "headers/file_reference.h"

extern void *memset(void *dst, int c, size_t n);
extern void file_location_get_full_path(int16_t location, const char *path, char *full_path);
extern const char *tag_name_strip_path(const char *name);

uint8_t file_open(file_reference *file, unsigned int flags)
{
    char full_path[304];
    memset(full_path, 0, 256);
    unsigned int access = 0;
    int opened = 0;
    file_location_get_full_path(*(unsigned short *)&file->data[6], &file->data[8], full_path);

    if ( flags & 1 )
        access = 0x80000000;
    if ( flags & 2 )
        access |= 0x40000000u;

    const char *leaf = tag_name_strip_path(full_path);
    const char *end = leaf;
    while ( *end++ )
        ;
    if ( (unsigned int)(end - leaf - 1) > 0x28 )
    {
        char *leaf_mutable = (char *)leaf;
        leaf_mutable[40] = 0;   /* truncate over-long leaf to 40 chars */
    }

    void *handle = CreateFileA(full_path, access, 0, 0, 3u, 0x80u, 0);
    if ( handle != (void *)-1 )
    {
        *(void **)&file->data[264] = handle;
        opened = 1;
    }

    if ( (unsigned char)opened && (flags & 4)
         && SetFilePointer(*(void **)&file->data[264], 0, 0, 2u) == (DWORD)-1 )
    {
        CloseHandle(*(void **)&file->data[264]);
        opened = 0;
        *(void **)&file->data[264] = 0;
    }

    if ( !(unsigned char)opened )
    {
        GetLastError();
        SetLastError(0);
    }
    return opened;
}
