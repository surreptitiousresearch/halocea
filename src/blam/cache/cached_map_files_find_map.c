/* cached_map_files_find_map @0x83754B58 — return the cached-map slot index whose header name matches (case-
 * insensitively) the given name, or -1 if none of the 6 slots match. */
#include <stdint.h>
#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"
extern int stricmp(const char *a, const char *b);
int16_t cached_map_files_find_map(const char *name)
{
    int index = 0;
    while ( stricmp(name, cache_file_globals_0.cached_map_files[index].header.name) )
    {
        index = (short)(index + 1);
        if ( index >= 6 )
            return -1;
    }
    return index;
}
