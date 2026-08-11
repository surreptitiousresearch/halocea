/* cache_file_map_exists @0x83754360 — test whether a map's cache file is present on disc by stripping the
 * path to the bare map name and reading its cache file header from the DVD. */

#include <stdint.h>
#include "headers/cache_file_header.h"

extern const char *tag_name_strip_path(const char *name);
extern uint8_t cache_file_read_header_from_dvd(const char *name, cache_file_header *header);

uint8_t cache_file_map_exists(const char *name)
{
    const char *bare_name = tag_name_strip_path(name);
    cache_file_header header;
    return cache_file_read_header_from_dvd(bare_name, &header);
}
