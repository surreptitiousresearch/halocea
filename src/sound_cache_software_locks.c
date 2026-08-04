/* sound_cache_software_locks @ 0x837E73D0 — true if the permutation is resident (has a cache block)
 * and that block currently carries at least one software lock/use (byte at +5 != 0). */

#include <stdint.h>
#include "headers/sound_permutation.h"
#include "headers/pc_sound_cache_globals.h"
#include "headers/cache_sound_datum.h"
#include "headers/data_array.h"

uint8_t sound_cache_software_locks(const sound_permutation *sound)
{
    int cache_block_index = sound->cache_block_index;
    if ( cache_block_index == -1 )
        return 0;

    cache_sound_datum *cache_entry =
        DATA_ARRAY_ELEMENT(pc_sound_cache_globals.cache_sounds, cache_sound_datum, cache_block_index);
    return cache_entry->software_reference_count != 0;
}
