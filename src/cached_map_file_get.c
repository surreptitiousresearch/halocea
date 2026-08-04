#include <stdint.h>
#include "headers/cached_map_file.h"
#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"


cached_map_file * cached_map_file_get(int16_t map_file_index)
{
    /* recovered: (char *)&cache_file_globals_0 + 2060 * map_file_index -> cached_map_files[map_file_index] */
    return &cache_file_globals_0.cached_map_files[map_file_index];
}
