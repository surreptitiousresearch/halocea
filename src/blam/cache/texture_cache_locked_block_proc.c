/* texture_cache_locked_block_proc @0x836F3F70 — the lruv cache's locked_block_proc for the texture
 * cache. Returns 1 when the block at the given index is LOCKED, i.e. not evictable: lruv_block_new
 * takes a non-zero return as `blocking` (bctrl @0x8371C774, then clrlwi r11,r3,24 @0x8371C778 /
 * cmplwi cr6,r11,0 @0x8371C780 / bne @0x8371C784 selecting 1 over 0).
 * The byte at +4 is texture_cache_texture::finished, not a lock count, and a block is locked exactly
 * while that flag is still 0 — a texture whose async read has not completed cannot be thrown away.
 * Proof: lbz r10,4(r11) @0x836F3F84; cntlzw r9,r10 @0x836F3F88; extrwi r3,r9,1,26 @0x836F3F8C
 * selects bit 5 of the leading-zero count, which is 1 only for cntlzw == 32, i.e. only for byte == 0.
 * Block records are a stride-16 datum array (clrlslwi r10,r3,16,4 @0x836F3F74). */

#include <stdint.h>
#include "headers/pc_texture_cache_globals.h"

/* returns via extrwi r3,r9,1,26 (single boolean bit); the lruv locked_block_proc slot is
   uint8_t(*)(int) — attested return is uint8_t, not BOOL/int */
uint8_t texture_cache_locked_block_proc(int block_index)
{
    /* recovered: (unsigned char *)textures->data + ((16*block_index)&0xFFFF0) -> DATA_ARRAY_ELEMENT (stride 16) */
    return DATA_ARRAY_ELEMENT(pc_texture_cache_globals.textures, unsigned char, block_index)[4] == 0;
}
