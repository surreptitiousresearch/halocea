#include <stdint.h>
#include "headers/blam_data_globals.h"
/* _rasterizer_decal_vertices_delete @0x83711058 — release the cached local-vertex block
 * backing a decal's geometry. (Underscore-prefixed implementation; the un-prefixed
 * rasterizer_decal_vertices_delete is the wrapper.) */

#include "headers/lruv_cache.h"
extern void lruv_block_delete(lruv_cache *cache, int block_index);

void _rasterizer_decal_vertices_delete(int cache_index)
{
    lruv_block_delete(local_vertex_cache, cache_index);
}
