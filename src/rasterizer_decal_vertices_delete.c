/* rasterizer_decal_vertices_delete @0x836A60D0 — thunk forwarding to the rasterizer's internal decal-vertex
 * cache release for the given cache slot. */

extern void _rasterizer_decal_vertices_delete(int cache_index);

void rasterizer_decal_vertices_delete(int cache_index)
{
    _rasterizer_decal_vertices_delete(cache_index);
}
