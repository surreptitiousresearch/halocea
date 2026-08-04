/* cache_file_read_header_from_dvd @0x83754298 — open the map's .map file directly off the DVD and read+verify
 * its 2048-byte header. Returns 1 if the header read and verified, 0 otherwise. */
#include <stdint.h>
#include <windows.h>
#include "headers/cache_file_header.h"
extern char *cache_files_map_directory(void);
extern const char *cache_files_root_directory(void);
extern int sprintf_0(char *string, const char *format, ...);
extern uint8_t cache_file_header_verify(const cache_file_header *header, const char *name, uint8_t fatal);
uint8_t cache_file_read_header_from_dvd(const char *name, cache_file_header *header)
{
    uint8_t verified = 0;
    const char *map_dir  = cache_files_map_directory();
    const char *root_dir = cache_files_root_directory();
    char path[304];
    sprintf_0(path, "%s%s%s.map", root_dir, map_dir, name);

    void *handle = CreateFileA(path, 0x80000000, 1u, 0, 3u, 0, 0);
    if ( handle != (void *)-1 )
    {
        unsigned int bytes_read;
        if ( ReadFile(handle, header, 0x800u, &bytes_read, 0) && bytes_read == 2048 )
            verified = cache_file_header_verify(header, path, 1u) != 0;
        CloseHandle(handle);
    }
    return verified;
}
