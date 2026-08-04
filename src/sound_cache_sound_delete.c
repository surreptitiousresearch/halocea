/* sound_cache_sound_delete @ 0x837E7378 — fully evict a permutation from the sound cache: unload it
 * from the FMOD layer, free its virtual cache block (LRU), and clear its resident pointers. */

#include <stdint.h>
#include "headers/sound_permutation.h"
#include "headers/pc_sound_cache_globals.h"

#include "headers/lruv_cache.h"
extern void unload_sound_fmod(sound_permutation *sound);
extern void lruv_block_delete(lruv_cache *cache, uint16_t block_index);

void sound_cache_sound_delete(sound_permutation *sound)
{
    int cache_block_index;

    unload_sound_fmod(sound);
    cache_block_index = sound->cache_block_index;
    if ( cache_block_index != -1 )
        lruv_block_delete(pc_sound_cache_globals.cache, cache_block_index);
    sound->cache_block_index = -1;
    sound->cache_base_address = 0;
}
