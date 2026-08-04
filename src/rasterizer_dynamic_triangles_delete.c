/* rasterizer_dynamic_triangles_delete 0x836A5DE8 (thunk) — tail-call thunk to the underscore implementation. */
extern void _rasterizer_dynamic_triangles_delete(int dynamic_triangle_buffer_index);

void rasterizer_dynamic_triangles_delete(int dynamic_triangle_buffer_index)
{
    _rasterizer_dynamic_triangles_delete(dynamic_triangle_buffer_index);
}
