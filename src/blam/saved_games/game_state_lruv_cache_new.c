/* game_state_lruv_cache_new @0x83684878 — carve a new LRUV cache out of the game-state CPU allocation
 * arena. Reserves lruv_allocation_size(maximum_block_count) bytes at the current arena high-water mark,
 * advances the allocation cursor, folds the reservation size into the arena's allocation-size checksum,
 * initializes the cache in place, and returns it. */

#include <stdint.h>
#include "headers/game_state_globals.h"
#include "headers/lruv_cache.h"

extern int lruv_allocation_size(int16_t maximum_block_count);
extern void lruv_initialize(lruv_cache *cache, const char *name, int page_count, int page_size_bits, int16_t maximum_block_count, void (*delete_block_proc)(int), uint8_t (*locked_block_proc)(int));
extern void crc_checksum_buffer(uint32_t *crc_reference, const void *buffer, int buffer_size);

lruv_cache *game_state_lruv_cache_new(
        const char     *name,
        int             page_count,
        int             page_size_bits,
        int             maximum_block_count,
        void          (*delete_block_proc)(int),
        uint8_t        (*locked_block_proc)(int))
{
    int allocation_size = lruv_allocation_size(maximum_block_count);
    lruv_cache *cache =
        (lruv_cache *)((char *)game_state_globals.base_address + game_state_globals.cpu_allocation_size);
    game_state_globals.cpu_allocation_size += allocation_size;
    crc_checksum_buffer(&game_state_globals.allocation_size_checksum, &allocation_size, 4);
    lruv_initialize(cache, name, page_count, page_size_bits, maximum_block_count,
                    delete_block_proc, locked_block_proc);
    return cache;
}
