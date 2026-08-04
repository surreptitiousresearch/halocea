/* rasterizer_dynamic_triangles_unlock 0x836A5DE0 (thunk) — tail-call thunk to the underscore implementation. */
extern void _rasterizer_dynamic_triangles_unlock(int dynamic_triangle_buffer_index);

void rasterizer_dynamic_triangles_unlock(int dynamic_triangle_buffer_index)
{
    _rasterizer_dynamic_triangles_unlock(dynamic_triangle_buffer_index);
}
