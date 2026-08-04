#include <stdint.h>
#include "headers/pc_sound_cache_globals.h"
#include "headers/cache_sound_datum.h"
#include "headers/data_array.h"

/* return type unsigned __int8 per DB funcs.prototype (matches pc_sound_cache_globals.h decl). */
uint8_t sound_cache_locked_block_proc(int block_index)
{
    cache_sound_datum *cache_entry =
        DATA_ARRAY_ELEMENT(pc_sound_cache_globals.cache_sounds, cache_sound_datum, block_index);

    if (!cache_entry->available)
        return 1;
    if (cache_entry->software_reference_count)
        return 1;
    if (cache_entry->hardware_reference_count != 0)
        return 1;
    return 0;
}
