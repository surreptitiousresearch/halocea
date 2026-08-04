/* sound_cache_close @0x837E7450 — unload every cached sound: for each permutation, unload its FMOD data,
 * delete its LRU cache block, and clear its cache fields; then invalidate the sound cache data array. */
#include <stdint.h>
#include "headers/pc_sound_cache_globals.h"
#include "headers/data_iterator.h"
#include "headers/sound_permutation.h"
#include "headers/cache_sound_datum.h"
#include "headers/data_array.h"
#include "headers/lruv_cache.h"
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern void unload_sound_fmod(sound_permutation *sound);
extern void lruv_block_delete(lruv_cache *cache, uint16_t block_index);
extern void data_make_invalid(data_array *data);
void sound_cache_close(void)
{
    data_iterator iterator;
    data_iterator_new(&iterator, pc_sound_cache_globals.cache_sounds);
    for ( void *datum = data_iterator_next(&iterator); datum; datum = data_iterator_next(&iterator) )
    {
        sound_permutation *permutation = ((cache_sound_datum *)datum)->sound;
        unload_sound_fmod(permutation);
        int cache_block_index = permutation->cache_block_index;
        if ( cache_block_index != -1 )
            lruv_block_delete(pc_sound_cache_globals.cache, cache_block_index);
        permutation->cache_block_index = -1;
        permutation->cache_base_address = 0;
    }
    data_make_invalid(pc_sound_cache_globals.cache_sounds);
}
