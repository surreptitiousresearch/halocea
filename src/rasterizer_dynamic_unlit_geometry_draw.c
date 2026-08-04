/* rasterizer_dynamic_unlit_geometry_draw @0x836A6360 — forwarding thunk to the real implementation. */

#include "headers/shader.h"
#include "headers/bitmap_data.h"
#include "headers/render_animation.h"
#include "headers/real_point3d.h"

extern void _rasterizer_dynamic_unlit_geometry_draw(const shader *shader, const bitmap_data *primary_map, const render_animation *animation, int dynamic_triangle_buffer_index, int dynamic_vertex_buffer_index, int triangle_count, const real_point3d *centroid, unsigned int geometry_flags);

void rasterizer_dynamic_unlit_geometry_draw(const shader *shader, const bitmap_data *primary_map,
                                             const render_animation *animation, int dynamic_triangle_buffer_index,
                                             int dynamic_vertex_buffer_index, int triangle_count,
                                             const real_point3d *centroid, unsigned int geometry_flags)
{
    _rasterizer_dynamic_unlit_geometry_draw(shader, primary_map, animation, dynamic_triangle_buffer_index,
                                            dynamic_vertex_buffer_index, triangle_count, centroid, geometry_flags);
}
