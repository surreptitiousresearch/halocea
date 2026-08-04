/* _rasterizer_dynamic_triangles_delete @0x83791430 — release a dynamic-triangle sub-buffer. The whole pool is
 * reset at frame begin, so individual deletes are no-ops in this build. */

void _rasterizer_dynamic_triangles_delete(int dynamic_triangle_buffer_index)
{
    (void)dynamic_triangle_buffer_index;
}
