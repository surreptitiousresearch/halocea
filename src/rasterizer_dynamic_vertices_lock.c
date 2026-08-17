/* rasterizer_dynamic_vertices_lock @0x836A5E00 */
extern void * _rasterizer_dynamic_vertices_lock(int dynamic_vertex_buffer_index);

void * rasterizer_dynamic_vertices_lock(int dynamic_vertex_buffer_index)
{
    return _rasterizer_dynamic_vertices_lock(dynamic_vertex_buffer_index);
}
