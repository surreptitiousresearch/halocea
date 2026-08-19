/* cache_files_precache_map_loaded @0x83754EC0 — true if the named map is already present in the cached-map
 * file list. Returns 1 when found, 0 otherwise (the arithmetic collapses the found index to a boolean). */
#include <stdint.h>

extern const char *tag_name_strip_path(const char *name);
extern int16_t cached_map_files_find_map(const char *name);
uint8_t cache_files_precache_map_loaded(const char *name)
{
    const char *stripped = tag_name_strip_path(name);
    int16_t map = cached_map_files_find_map(stripped);
    return map + 1 - (map + (map == -1));
}
