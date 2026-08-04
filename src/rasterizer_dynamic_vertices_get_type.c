#include <stdint.h>

extern int16_t _rasterizer_dynamic_vertices_get_type(int dynamic_vertex_buffer_index);

int16_t rasterizer_dynamic_vertices_get_type(int dynamic_vertex_buffer_index)
{
    return _rasterizer_dynamic_vertices_get_type(dynamic_vertex_buffer_index);
}
