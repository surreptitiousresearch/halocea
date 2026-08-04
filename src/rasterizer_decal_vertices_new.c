extern int _rasterizer_decal_vertices_new(int cache_size);

int rasterizer_decal_vertices_new(int cache_size)
{
    return _rasterizer_decal_vertices_new(cache_size);
}
