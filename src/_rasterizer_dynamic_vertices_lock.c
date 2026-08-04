/* _rasterizer_dynamic_vertices_lock @0x83791528 — CPU-locks one of the dynamic vertex buffer slots for
 * writing, resetting its base offset/start index to the freshly locked region. Returns nullptr for the
 * "no buffer" sentinel index (-1). */

#include <stdint.h>
#include "headers/dynamic_vertices.h"

extern int16_t rasterizer_geometry_get_vertex_size(int16_t type);
extern unsigned int hcex_dyn_vbuf_lock(int vertex_count, unsigned int vertex_size, void **data);

void * _rasterizer_dynamic_vertices_lock(int dynamic_vertex_buffer_index)
{
    if (dynamic_vertex_buffer_index == -1)
        return nullptr;

    dynamic_vertex_buffer *buffer = &dynamic_vertices.buffers[dynamic_vertex_buffer_index];
    unsigned int vertex_size = rasterizer_geometry_get_vertex_size(buffer->type);
    buffer->vertex_buf_offset = hcex_dyn_vbuf_lock(buffer->vertex_count, vertex_size, &buffer->data);
    buffer->vertex_start_index = 0;
    return buffer->data;
}
