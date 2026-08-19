/* structure_render_fog_screen @0x837C6D70 — render the screen-space fog pass over the BSP environment
 * surfaces, in two sub-passes (mode 0 then mode 1). No-op when the environment geometry is invalid. */

#include <stdint.h>
#include "headers/structure_render_globals.h"
#include "headers/render_globals.h"
#include "headers/structure_render_pass.h"

extern void rasterizer_environment_fog_screen_begin(int16_t pass);
extern void rasterizer_environment_fog_screen_end(void);
extern void rasterizer_environment_fog_screen_draw(const struct shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const struct vertex_buffer *vertex_buffer);

void structure_render_fog_screen(void)
{
    if (!structure_render_globals.environment_geometry_valid)
        return;

    rasterizer_environment_fog_screen_begin(0);
    structure_render_pass(render.environment_surface_indices, render.environment_surface_count,
                          structure_render_globals.environment_triangles_index,
                          nullptr, rasterizer_environment_fog_screen_draw, nullptr, nullptr);
    rasterizer_environment_fog_screen_end();

    rasterizer_environment_fog_screen_begin(1);
    structure_render_pass(render.environment_surface_indices, render.environment_surface_count,
                          structure_render_globals.environment_triangles_index,
                          nullptr, rasterizer_environment_fog_screen_draw, nullptr, nullptr);
    rasterizer_environment_fog_screen_end();
}
