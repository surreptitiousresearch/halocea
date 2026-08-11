/* sound_cache_flush @0x837E74D0 — evict every currently-loaded, non-in-use, non-locked cached sound
 * permutation (datum+5/+6 both clear): unload its FMOD data, delete its LRU cache block, and clear its cache
 * fields. Unlike sound_cache_close.c, the data_array itself is left valid — only matching entries are evicted. */

#include <stdint.h>
#include "headers/pc_sound_cache_globals.h"
#include "headers/data_iterator.h"
#include "headers/sound_permutation.h"
#include "headers/cache_sound_datum.h"

#include "headers/lruv_cache.h"
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern void unload_sound_fmod(sound_permutation *sound);
extern void lruv_block_delete(lruv_cache *cache, int block_index);

void sound_cache_flush(void)
{
    if ( !pc_sound_cache_globals.cache_sounds || !pc_sound_cache_globals.cache_sounds->valid )
        return;

    data_iterator iterator;
    data_iterator_new(&iterator, pc_sound_cache_globals.cache_sounds);

    for ( cache_sound_datum *datum = data_iterator_next(&iterator);
          datum;
          datum = data_iterator_next(&iterator) )
    {
        if ( datum->software_reference_count || datum->hardware_reference_count )
            continue;

        sound_permutation *permutation = datum->sound;
        unload_sound_fmod(permutation);

        int cache_block_index = permutation->cache_block_index;
        if ( cache_block_index != -1 )
            lruv_block_delete(pc_sound_cache_globals.cache, cache_block_index);

        permutation->cache_block_index = -1;
        permutation->cache_base_address = 0;
    }
}
