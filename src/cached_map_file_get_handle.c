#include <stdint.h>
#include "headers/cached_map_file.h"
#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"


void * cached_map_file_get_handle(int16_t map_file_index)
{
    return cache_file_globals_0.cached_map_files[map_file_index].handle;
}
