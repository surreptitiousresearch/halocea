/* structure_render_transparent_geometry @0x837C6C80 — render the transparent-geometry pass over the BSP
 * environment surfaces. Unlike the opaque passes this supplies the draw_transparent_triangles callback
 * (the trailing structure_render_pass argument) rather than draw_triangles. No-op when the environment
 * geometry for this frame is not valid. */

#include <stdint.h>
#include "headers/structure_render_globals.h"
#include "headers/render_globals.h"
#include "headers/structure_render_pass.h"
#include "headers/render_lighting.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
extern void rasterizer_environment_transparent_geometry_begin(void);
extern void rasterizer_environment_transparent_geometry_end(void);
extern void rasterizer_environment_transparent_geometry_submit(const struct shader *shader, int16_t shader_permutation_index, const struct bitmap_data *lightmap, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const struct vertex_buffer *vertex_buffers, const real_point3d *centroid, const struct real_plane3d *plane, const real_vector3d *offset, const struct render_lighting *render_lighting, unsigned int geometry_flags);          /* real_point3d/real_vector3d are union tags */

void structure_render_transparent_geometry(void)
{
    if (!structure_render_globals.environment_geometry_valid)
        return;

    rasterizer_environment_transparent_geometry_begin();
    structure_render_pass(render.environment_surface_indices, render.environment_surface_count,
                          structure_render_globals.environment_triangles_index,
                          nullptr, nullptr, nullptr,
                          rasterizer_environment_transparent_geometry_submit);
    rasterizer_environment_transparent_geometry_end();
}
