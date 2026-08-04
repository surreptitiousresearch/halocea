#include "headers/dynamic_vertices.h"

int _rasterizer_dynamic_vertices_get_type(int dynamic_vertex_buffer_index)
{
    if (dynamic_vertex_buffer_index != -1)
        return (unsigned short)dynamic_vertices.buffers[dynamic_vertex_buffer_index].type;
    return -1;
}
