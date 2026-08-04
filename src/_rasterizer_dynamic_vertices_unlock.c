#include "headers/dynamic_vertices.h"

extern void hcex_dyn_vbuf_unlock(int vertex_count);

void _rasterizer_dynamic_vertices_unlock(int dynamic_vertex_buffer_index)
{
    if (dynamic_vertex_buffer_index != -1)
        hcex_dyn_vbuf_unlock(dynamic_vertices.buffers[dynamic_vertex_buffer_index].vertex_count);
}
