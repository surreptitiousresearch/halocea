/* cache_file_open @0x83754E60 — open the cached map file for the given scenario: locate its slot, reset the
 * pending-request ring, set it as the open map, and copy out its header. Always returns 1. */
#include <stdint.h>
#include <string.h>
#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"
extern int16_t cached_map_files_find_map(const char *name);
/* memcpy declared by <string.h> */
uint8_t cache_file_open(const char *scenario_name, cache_file_header *header)
{
    int16_t map = cached_map_files_find_map(scenario_name);
    memset(cache_file_globals_0.requests, 0, 0x6000u);
    cache_file_globals_0.open_map_file_index = map;
    memcpy(header, &cache_file_globals_0.cached_map_files[map].header, sizeof(cache_file_header));
    return 1;
}
