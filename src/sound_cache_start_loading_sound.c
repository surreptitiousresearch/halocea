/* sound_cache_start_loading_sound @0x837E7B50 — reserve a cache block for `sound` and begin loading it: the
 * requested size is the raw sample size, except duplicate_compression==1 also stashes that size in
 * unused0[0] (a second copy the decoder consults later). If no cache block is free, dumps cache stats to
 * disk and deliberately crashes (`*(int*)0 = 1`, a debug-assert-by-null-write — this should never happen in
 * practice). Otherwise links a new cache_sounds datum at the block's page offset and marks it loading. */

#include <stdint.h>
#include "headers/pc_sound_cache_globals.h"
#include "headers/sound_permutation.h"
#include "headers/cache_sound_datum.h"

#include "headers/data_array.h"
#include "headers/lruv_cache.h"
extern int lruv_block_new(lruv_cache *cache, int size);
extern void sound_cache_dump_to_file(void);
extern int datum_new_at_index(data_array *data, int index);

void sound_cache_start_loading_sound(sound_permutation *sound, uint8_t blocking)
{
    int size = 0;

    if ( sound->duplicate_compression == 1 )
    {
        size = sound->samples.size;
        sound->unused0[0] = size;
    }
    else if ( sound->duplicate_compression == 3 || !sound->duplicate_compression )
    {
        size = sound->samples.size;
    }

    int block_index = lruv_block_new(pc_sound_cache_globals.cache, size);

    if ( block_index == -1 )
    {
        sound_cache_dump_to_file();
        *(int *)0 = 1;
    }
    else
    {
        datum_new_at_index(pc_sound_cache_globals.cache_sounds, block_index);
        cache_sound_datum *cache_sounds = (cache_sound_datum *)pc_sound_cache_globals.cache_sounds->data;
        sound->cache_block_index = block_index;
        cache_sound_datum *block = &cache_sounds[(uint16_t)block_index];
        sound->cache_base_address = 0;
        block->sound = sound;
        block->request_index = 0;
        block->available = 1;
    }
}
