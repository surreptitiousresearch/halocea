/* cache_files_precache_is_copying_map @0x83753DE8 — true if the background copy currently in progress is
 * copying the map with the given name (path-stripped, compared against copying_to_map_file_name). */

#include <stdint.h>
#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"
extern const char *tag_name_strip_path(const char *name);

uint8_t cache_files_precache_is_copying_map(const char *name)
{
    if ( (unsigned short)cache_file_globals_0.copying_to_map_file_index == 0xFFFF )
        return 0;

    const char *stripped = tag_name_strip_path(name);
    const char *copying = cache_file_globals_0.copying_to_map_file_name;
    int diff;
    do
    {
        int c = (unsigned char)*copying;
        diff = c - (unsigned char)*stripped;
        if ( c == 0 )
            break;
        ++copying;
        ++stripped;
    }
    while ( !diff );

    if ( diff )
        return 0;
    return 1;
}
