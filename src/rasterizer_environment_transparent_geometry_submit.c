/* rasterizer_environment_transparent_geometry_submit @0x836A62D8 */
#include <stdint.h>
/* rasterizer_environment_transparent_geometry_submit 0x836A62D8 (thunk) — tail-call thunk to the underscore
 * implementation that enqueues one environment transparent-geometry surface batch into the sorted group list. */

struct shader;
struct bitmap_data;
struct vertex_buffer;
struct real_point3d;
struct real_plane3d;
struct real_vector3d;
struct render_lighting;

#include "headers/shader.h"
#include "headers/bitmap_data.h"
#include "headers/vertex_buffer.h"
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"
#include "headers/real_vector3d.h"
#include "headers/render_lighting.h"
extern void _rasterizer_environment_transparent_geometry_submit(const shader *shader, int16_t shader_permutation_index, const bitmap_data *lightmap, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffers, const real_point3d *centroid, const real_plane3d *plane, const real_vector3d *offset, const render_lighting *render_lighting, unsigned int geometry_flags);

void rasterizer_environment_transparent_geometry_submit(const struct shader *shader, int16_t shader_permutation_index, const struct bitmap_data *lightmap, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const struct vertex_buffer *vertex_buffers, const struct real_point3d *centroid, const struct real_plane3d *plane, const struct real_vector3d *offset, const struct render_lighting *render_lighting, unsigned int geometry_flags)
{
    _rasterizer_environment_transparent_geometry_submit(
        shader, shader_permutation_index, lightmap, dynamic_triangle_buffer_index, first_triangle_index,
        triangle_count, vertex_buffers, centroid, plane, offset, render_lighting, geometry_flags);
}
