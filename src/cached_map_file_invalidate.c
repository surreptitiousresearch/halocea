#include <stdint.h>
#include "headers/cache_file_globals.h"

void cached_map_file_invalidate(int16_t map_file_index)
{
    cache_file_globals_0.cached_map_files[map_file_index].handle = (void *)-1;
}
