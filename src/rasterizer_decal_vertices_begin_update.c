/* rasterizer_decal_vertices_begin_update @0x83710FB8 — idles the decal vertex LRU cache at the start
 * of a decal vertex update (the matching end_update is a no-op on this platform). */

#include "headers/lruv_cache.h"
#include "headers/blam_data_globals.h"

extern void lruv_idle(lruv_cache *cache);

void rasterizer_decal_vertices_begin_update(void)
{
    lruv_idle(local_vertex_cache);
}
