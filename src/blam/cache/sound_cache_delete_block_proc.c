/* sound_cache_delete_block_proc @0x837E7650 — lruv_cache delete_block_proc for the sound cache: unloads
 * and unlinks the sound_permutation pointer held in the block's 16-byte cache_sound_datum, then frees the
 * block's cache-sounds datum. */

#include "headers/sound_permutation.h"
#include "headers/pc_sound_cache_globals.h"
#include "headers/cache_sound_datum.h"

#include "headers/data_array.h"
extern void unload_sound_fmod(sound_permutation *sound);
extern void datum_delete(data_array *data, int index);

void sound_cache_delete_block_proc(int block_index)
{
    cache_sound_datum *cache_entry =
        DATA_ARRAY_ELEMENT(pc_sound_cache_globals.cache_sounds, cache_sound_datum, block_index);
    sound_permutation *sound = cache_entry->sound;

    unload_sound_fmod(sound);
    sound->cache_block_index = -1;
    sound->cache_base_address = 0;
    datum_delete(pc_sound_cache_globals.cache_sounds, block_index);
}
