/* file_reference_get_name @0x83767A58 — render a file_reference's name into `name`, selecting components by
 * `flags`: bit 0 = directory, bit 1 = base name, bit 2 = file name, bit 3 = extension. Resolves the full path
 * first, splits it, then appends the requested pieces. */

#include <stdint.h>
#include "headers/file_reference.h"

extern void *memset(void *dst, int value, unsigned int n);
extern void file_location_get_full_path(int16_t location, const char *path, char *full_path);
extern void file_path_split(char *path, char **directory, char **parent_directory, char **filename, char **extension, uint8_t has_filename);
extern void file_path_add_name(char *path, const char *name);
extern void file_path_add_extension(char *path, const char *extension);

char *file_reference_get_name(const file_reference *reference, unsigned int flags, char *name)
{
    char *directory;
    char *base_name;
    char *file_name;
    char *extension;
    char full_path[304];

    memset(full_path, 0, 256);
    file_location_get_full_path(*(uint16_t *)&reference->data[6], &reference->data[8], full_path);
    file_path_split(full_path, &base_name, &file_name, &directory, &extension, reference->data[4] & 1);

    *name = 0;
    if ( (flags & 1) != 0 )
        file_path_add_name(name, base_name);
    if ( (flags & 2) != 0 )
        file_path_add_name(name, file_name);
    if ( (flags & 4) != 0 )
        file_path_add_name(name, directory);
    if ( (flags & 8) != 0 )
    {
        file_path_add_extension(name, extension);
        return name;
    }
    return name;
}
