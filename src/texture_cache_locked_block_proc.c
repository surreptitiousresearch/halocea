/* texture_cache_locked_block_proc @0x836F3F70 — predicate for the texture cache's locked-block
 * scan: returns true when the cache block at the given index is unlocked (lock count byte == 0).
 * Block records are a stride-16 datum array; the lock byte lives at +4. */

#include <stdint.h>
#include "headers/pc_texture_cache_globals.h"

/* returns via extrwi r3,r9,1,26 (single boolean bit); the lruv locked_block_proc slot is
   uint8_t(*)(int) — attested return is uint8_t, not BOOL/int */
uint8_t texture_cache_locked_block_proc(int block_index)
{
    /* recovered: (unsigned char *)textures->data + ((16*block_index)&0xFFFF0) -> DATA_ARRAY_ELEMENT (stride 16) */
    return DATA_ARRAY_ELEMENT(pc_texture_cache_globals.textures, unsigned char, block_index)[4] == 0;
}
