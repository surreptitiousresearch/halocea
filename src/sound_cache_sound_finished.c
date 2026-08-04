/* sound_cache_sound_finished @ 0x837E7410 — release one software hold on a permutation's cache
 * block: decrement the block's software_reference_count (byte at +5 of the 16-byte cache-sound
 * record). */

#include "headers/sound_permutation.h"
#include "headers/cache_sound_datum.h"
#include "headers/pc_sound_cache_globals.h"
#include "headers/data_array.h"

void sound_cache_sound_finished(sound_permutation *sound)
{
    cache_sound_datum *entry = DATA_ARRAY_ELEMENT(pc_sound_cache_globals.cache_sounds, cache_sound_datum,
                                                  sound->cache_block_index);
    if ( entry )
        --entry->software_reference_count;
}
