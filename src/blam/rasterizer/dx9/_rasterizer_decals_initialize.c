/* _rasterizer_decals_initialize @0x83710D60 — allocate the shared D3D decal vertex buffer, and (once) the
 * decal vertex LRUV cache.
 *
 * Deviation: an earlier reconstruction typed this as `lruv_cache *` returning local_vertex_cache, but the
 * disassembly never does `mr r3, local_vertex_cache` before blr — on the "cache already initialized" path
 * (bne @0x83710D9C -> 0x83710DCC) r3 still holds the dlMalloc buffer, on the fall-through path r3 is the
 * game_state_lruv_cache_new result. The return value is residue, and the sole caller
 * (rasterizer_decals_initialize @0x836A5FC0) tail-threads r3 without consuming it. Attested void. */

#include <stdint.h>
#include "headers/lruv_cache.h"
#include "headers/blam_data_globals.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void rasterizer_decal_vertices_purge_proc(int block_index);
extern uint8_t rasterizer_decal_vertices_locked_proc(int block_index);
extern lruv_cache *game_state_lruv_cache_new(const char *name, int page_count, int page_size_bits, int maximum_block_count, void (*delete_block_proc)(int), uint8_t (*locked_block_proc)(int));

void _rasterizer_decals_initialize(void)
{
    local_d3d_vertex_buffer = dlMalloc(
        0x3C000u, "D:\\Projects\\code\\HCEX\\sources\\rasterizer\\dx9\\rasterizer_dx9_decals.c", 0x61u);

    if ( !local_vertex_cache )
    {
        local_vertex_cache = game_state_lruv_cache_new(
            "decal vertex cache", 2560, 6, 2048, rasterizer_decal_vertices_purge_proc, rasterizer_decal_vertices_locked_proc);
    }
}
