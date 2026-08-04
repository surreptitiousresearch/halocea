#include "headers/lruv_cache.h"
#include "headers/blam_data_globals.h"

#include "headers/lruv_cache.h"
extern int lruv_block_new(lruv_cache *cache, int size);

/* DB name is _rasterizer_decal_vertices_new (0x83710FD0); rasterizer_decal_vertices_new (0x836A60B8) is a thunk to it */
int _rasterizer_decal_vertices_new(int cache_size)
{
    return lruv_block_new(local_vertex_cache, cache_size);
}
