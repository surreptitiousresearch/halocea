#include "headers/lruv_cache.h"
#include "headers/blam_data_globals.h"

extern void (*rasterizer_decal_vertices_purge_proc)(int);
extern unsigned char (*rasterizer_decal_vertices_locked_proc)(int);
extern void lruv_update_function_pointers(lruv_cache *cache,
                                           void (*delete_block_proc)(int),
                                           unsigned char (*locked_block_proc)(int));

void _rasterizer_decals_update_function_pointers(void)
{
    lruv_update_function_pointers(
        local_vertex_cache,
        rasterizer_decal_vertices_purge_proc,
        rasterizer_decal_vertices_locked_proc);
}
