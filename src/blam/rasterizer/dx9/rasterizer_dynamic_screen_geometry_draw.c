/* rasterizer_dynamic_screen_geometry_draw @0x836A6390 — forwarding thunk to the real implementation. */

#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"

extern void _rasterizer_dynamic_screen_geometry_draw(const rasterizer_dynamic_screen_geometry_parameters *parameters, int dynamic_triangle_buffer_index, int dynamic_vertex_buffer_index, int triangle_count);

void rasterizer_dynamic_screen_geometry_draw(const rasterizer_dynamic_screen_geometry_parameters *parameters,
                                              int dynamic_triangle_buffer_index, int dynamic_vertex_buffer_index,
                                              int triangle_count)
{
    _rasterizer_dynamic_screen_geometry_draw(parameters, dynamic_triangle_buffer_index, dynamic_vertex_buffer_index,
                                             triangle_count);
}
