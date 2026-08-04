/* _rasterizer_decal_vertices_lock @0x83710FE0 — address of a decal-cache block's vertex data within the
 * decal dynamic vertex buffer (16 bytes/vertex, so the byte offset is 1.5x the block's page address), and
 * marks the rasterizer as mid-lock so the frame's flush logic knows a CPU write is in flight. */

#include <stdint.h>
#include "headers/lruv_cache.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_lock_operation.h"
#include "headers/blam_data_globals.h"


extern unsigned int lruv_block_get_address(const lruv_cache *cache, uint16_t block_index);

void *_rasterizer_decal_vertices_lock(int cache_index, int cache_size)
{
    (void)cache_size;
    void *result = (char *)local_d3d_vertex_buffer
            + ((3 * lruv_block_get_address(local_vertex_cache, cache_index)) >> 1);
    rasterizer_globals.current_lock_operation = _rasterizer_lock_decal_update;
    return result;
}
