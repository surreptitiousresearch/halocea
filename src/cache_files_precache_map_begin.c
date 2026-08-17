/* cache_files_precache_map_begin @0x83754EF8 — begin caching the named map: if not already cached, read its
 * header from DVD, reserve a free cached-map slot sized for it, open the destination .map file, and read its
 * header. Returns 1 on success (or already cached), 0 on failure (raising a damaged-media error if blocking).
 *
 * CAVEAT: as-shipped — the binary never guards cached_map_files_find_free_map's -1 return: the result goes
 * straight through extsh/mulli into the slot address (extsh r10,r3 @0x83754F54; mulli r10,r10,0x80C
 * @0x83754F5C; memset @0x83754F78) with no cmpwi -1 between the bl @0x83754F50 and the memset, so a full
 * cache indexes cached_map_files[-1]. Reproduced verbatim. */

/* removed <windows.h>: canonical blam_data_globals.h provides self-contained Win32 type shims (tagRECT/HWND__/_OVERLAPPED/_FILETIME); system windows.h collided. Win32 fn protos resolve via project headers/implicit-decl. */
#include <stdint.h>
#include <string.h>
#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"


extern const char *tag_name_strip_path(const char *name);
extern int16_t cached_map_files_find_map(const char *name);
extern uint8_t cache_file_read_header_from_dvd(const char *name, cache_file_header *header);
extern int16_t cached_map_files_find_free_map(int size, int16_t scenario_type);
extern char *cache_files_map_directory(void);
extern const char *cache_files_root_directory(void);
extern int sprintf_0(char *string, const char *format, ...);
extern void cached_map_file_read_header(int16_t map_file_index);
extern void display_error_damaged_media(void);
extern void *CreateFileA(const char *name, unsigned int access, unsigned int share, void *security,
    unsigned int creation, unsigned int flags, void *template_file);

uint8_t cache_files_precache_map_begin(const char *name, uint8_t blocking)
{
    const char *stripped = tag_name_strip_path(name);

    if ( cached_map_files_find_map(tag_name_strip_path(name)) != -1 )
        return 1;

    cache_file_header header;
    if ( cache_file_read_header_from_dvd(stripped, &header) )
    {
        short free_map = cached_map_files_find_free_map(header.size, header.scenario_type);
        memset(&cache_file_globals_0.cached_map_files[free_map].header, 0,
               sizeof(cache_file_globals_0.cached_map_files[free_map].header));

        const char *map_dir  = cache_files_map_directory();
        const char *root_dir = cache_files_root_directory();
        char path[256];
        sprintf_0(path, "%s%s%s.map", root_dir, map_dir, stripped);

        cache_file_globals_0.cached_map_files[free_map].handle =
            CreateFileA(path, 0x80000000, 1u, 0, 4u, 0x48000080u, 0);
        cached_map_file_read_header(free_map);
        return 1;
    }

    if ( blocking )
        display_error_damaged_media();
    return 0;
}
