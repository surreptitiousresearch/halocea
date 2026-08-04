#include <stdint.h>
#include "headers/dynamic_vertices.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_limits.h"

extern int16_t rasterizer_geometry_get_vertex_size(int16_t type);

int _rasterizer_dynamic_vertices_new(int16_t type, int count)
{
    if (count <= 0)
        return -1;
    if (dynamic_vertices.groups[0].vertex_count >= dynamic_vertices.groups[0].max_vertex_count - count
        || dynamic_vertices.buffer_count >= RASTERIZER_MAXIMUM_DYNAMIC_VERTEX_BUFFERS - 1)
    {
        if (!warned_5)
            warned_5 = 1;
        return -1;
    }
    rasterizer_geometry_get_vertex_size(type);
    int index = dynamic_vertices.buffer_count;
    dynamic_vertex_buffer *buf = &dynamic_vertices.buffers[index];
    buf->type = type;
    buf->vertex_start_index = dynamic_vertices.groups[0].vertex_count;
    buf->vertex_buf_offset = 0;
    buf->vertex_count = count;
    ++dynamic_vertices.buffer_count;
    dynamic_vertices.groups[0].vertex_count += count;
    return index;
}
