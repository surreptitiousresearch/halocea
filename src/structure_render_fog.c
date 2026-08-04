/* structure_render_fog @0x837C6CF8 — render the fog pass over the BSP environment surfaces. No-op when the
 * environment geometry for this frame is not valid. */

#include <stdint.h>
#include "headers/structure_render_globals.h"
#include "headers/render_globals.h"
#include "headers/structure_render_pass.h"

extern void rasterizer_environment_fog_begin(void);
extern void rasterizer_environment_fog_end(void);
extern void rasterizer_environment_fog_draw(const struct shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const struct vertex_buffer *vertex_buffer);

void structure_render_fog(void)
{
    if (!structure_render_globals.environment_geometry_valid)
        return;

    rasterizer_environment_fog_begin();
    structure_render_pass(render.environment_surface_indices, render.environment_surface_count,
                          structure_render_globals.environment_triangles_index,
                          nullptr, rasterizer_environment_fog_draw, nullptr, nullptr);
    rasterizer_environment_fog_end();
}
