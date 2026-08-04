extern void *_rasterizer_decal_vertices_lock(int cache_index, int cache_size);

void *rasterizer_decal_vertices_lock(int cache_index, int cache_size)
{
    return _rasterizer_decal_vertices_lock(cache_index, cache_size);
}
