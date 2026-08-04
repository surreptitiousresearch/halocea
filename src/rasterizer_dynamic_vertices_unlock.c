extern void _rasterizer_dynamic_vertices_unlock(int dynamic_vertex_buffer_index);

void rasterizer_dynamic_vertices_unlock(int dynamic_vertex_buffer_index)
{
    _rasterizer_dynamic_vertices_unlock(dynamic_vertex_buffer_index);
}
