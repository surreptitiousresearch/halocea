#include <stdint.h>
/* cache_file_exists @0x83754038 — test whether the .map cache file for the given map name exists on disk. */

extern const char *tag_name_strip_path(const char *name);
extern char *cache_files_map_directory(void);
extern const char *cache_files_root_directory(void);
extern int sprintf_0(char *string, const char *format, ...);
extern int _access(const char *path, int mode);

uint8_t cache_file_exists(const char *name)
{
    const char *stripped = tag_name_strip_path(name);
    const char *map_dir  = cache_files_map_directory();
    const char *root_dir = cache_files_root_directory();
    char path[264];

    sprintf_0(path, "%s%s%s.map", root_dir, map_dir, stripped);
    return _access(path, 0) == 0;
}
