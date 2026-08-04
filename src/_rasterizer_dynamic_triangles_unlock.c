/* _rasterizer_dynamic_triangles_unlock @0x83791400 — unlock a previously-allocated dynamic-triangle sub-buffer
 * so its index data can be used by the GPU, forwarding the triangle count to the hcex index-buffer backend. */

#include "headers/dynamic_triangles.h"

extern dynamic_triangles_struct dynamic_triangles;
extern void hcex_dyn_ibuf_unlock(int triangle_count);

void _rasterizer_dynamic_triangles_unlock(int dynamic_triangle_buffer_index)
{
    if (dynamic_triangle_buffer_index != -1)
        hcex_dyn_ibuf_unlock(dynamic_triangles.buffers[dynamic_triangle_buffer_index].triangle_count);
}
