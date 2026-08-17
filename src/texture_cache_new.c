/* texture_cache_new @ 0x836F4150
   Allocates the PC texture descriptor array and its LRU/validation cache,
   wiring the delete/locked block callbacks. */

#include <stdint.h>
#include "headers/pc_texture_cache_globals.h"

extern data_array *data_new(const char *name, int16_t maximum_count, int16_t size);
extern lruv_cache *lruv_new(const char *name, int page_count, int page_size_bits, int16_t maximum_block_count, void (*delete_block_proc)(int), uint8_t (*locked_block_proc)(int));
extern void texture_cache_delete_block_proc(int block_index);
extern uint8_t texture_cache_locked_block_proc(int block_index);

/* DEVIATION: the lruv_new result at blr is residue already stored to
   pc_texture_cache_globals.cache (stw @0x836F41A8); sole caller ignores r3 — void. */
void texture_cache_new(void)
{
    pc_texture_cache_globals.textures = data_new("pc texture", 1280, 16);
    pc_texture_cache_globals.cache = lruv_new("pc texture cache", 1280, 14, 1280,
                                              texture_cache_delete_block_proc,
                                              texture_cache_locked_block_proc);
}
