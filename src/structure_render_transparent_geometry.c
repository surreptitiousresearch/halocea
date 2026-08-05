/* structure_render_transparent_geometry @0x837C6C80 — render the transparent-geometry pass over the BSP
 * environment surfaces. Unlike the opaque passes this supplies the draw_transparent_triangles callback
 * (the trailing structure_render_pass argument) rather than draw_triangles. No-op when the environment
 * geometry for this frame is not valid. */

#include <stdint.h>
#include "headers/structure_render_globals.h"
#include "headers/render_globals.h"
#include "headers/structure_render_pass.h"

extern void rasterizer_environment_transparent_geometry_begin(void);
extern void rasterizer_environment_transparent_geometry_end(void);
extern void rasterizer_environment_transparent_geometry_submit(
    const struct shader *, int16_t, const struct bitmap_data *, int, int, int, const struct vertex_buffer *,
    const union real_point3d *, const struct real_plane3d *, const union real_vector3d *,
    const struct render_lighting *, unsigned int);   /* real_point3d/real_vector3d are union tags */

void structure_render_transparent_geometry(void)
{
    if (!structure_render_globals.environment_geometry_valid)
        return;

    rasterizer_environment_transparent_geometry_begin();
    structure_render_pass(render.environment_surface_indices, render.environment_surface_count,
                          structure_render_globals.environment_triangles_index,
                          nullptr, nullptr, nullptr,
                          /* cast the real 11-arg impl down to the 8-arg callback this caller supplies */
                          (void (*)(const struct shader *, int16_t, struct bitmap_data *, int, int, int,
                                    const struct vertex_buffer *, const union real_point3d *))
                              rasterizer_environment_transparent_geometry_submit);
    rasterizer_environment_transparent_geometry_end();
}
