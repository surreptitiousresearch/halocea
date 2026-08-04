/* structure_render_reflection_mirrors @0x837C6B90 — render the reflection-mirror pass over the BSP
 * environment surfaces. No-op when the environment geometry for this frame is not valid. */

#include <stdint.h>
#include "headers/structure_render_globals.h"
#include "headers/render_globals.h"
#include "headers/structure_render_pass.h"

extern void rasterizer_environment_reflection_mirrors_begin(void);
extern void rasterizer_environment_reflection_mirrors_end(void);
extern void rasterizer_environment_reflection_mirror_draw(const struct shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const struct vertex_buffer *vertex_buffer);

void structure_render_reflection_mirrors(void)
{
    if (!structure_render_globals.environment_geometry_valid)
        return;

    rasterizer_environment_reflection_mirrors_begin();
    structure_render_pass(render.environment_surface_indices, render.environment_surface_count,
                          structure_render_globals.environment_triangles_index,
                          nullptr, rasterizer_environment_reflection_mirror_draw, nullptr, nullptr);
    rasterizer_environment_reflection_mirrors_end();
}
