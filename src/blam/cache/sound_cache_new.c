/* sound_cache_new @ 0x837E7AA8
   Allocates the PC sound descriptor array and its LRU/validation cache. The
   cache page count is derived from sound_cache_size (MB -> 4KB pages:
   <<20 then >>12). */

#include <stdint.h>
#include "headers/pc_sound_cache_globals.h"

extern data_array *data_new(const char *name, int16_t maximum_count, int16_t size);
extern lruv_cache *lruv_new(const char *name, int page_count, int page_size_bits, int16_t maximum_block_count, void (*delete_block_proc)(int), uint8_t (*locked_block_proc)(int));

/* Attested void: r3 at blr is the leftover lruv_new return (no re-computation),
   the sole caller sound_initialize ignores it, and the created object is published
   via pc_sound_cache_globals.cache. */
void sound_cache_new(void)
{
    pc_sound_cache_globals.cache_sounds = data_new("pc sound", 512, 16);

    int page_count = (unsigned short)sound_cache_size << 20 >> 12; /* MB -> 4KB pages */
    sound_cache_page_count = page_count;

    lruv_cache *cache = lruv_new("pc sound cache", page_count, 12, 512,
                                 sound_cache_delete_block_proc,
                                 sound_cache_locked_block_proc);
    pc_sound_cache_globals.cache = cache;
    pc_sound_cache_globals.bInitialized = 1;
}
