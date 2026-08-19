/* structure_render_diffuse_texture @0x837C69E8 — render the diffuse-texture pass over the BSP environment
 * surfaces. No-op when the environment geometry for this frame is not valid.
 *
 * Typed void: the `return result` on the geometry-invalid path is the decompiler reading r3 uninitialized. */

#include <stdint.h>
#include "headers/structure_render_globals.h"
#include "headers/render_globals.h"
#include "headers/structure_render_pass.h"

extern void rasterizer_environment_diffuse_textures_begin(void);
extern void rasterizer_environment_diffuse_textures_end(void);
extern void rasterizer_environment_diffuse_texture_draw(const struct shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const struct vertex_buffer *vertex_buffer);

void structure_render_diffuse_texture(void)
{
    if (!structure_render_globals.environment_geometry_valid)
        return;

    rasterizer_environment_diffuse_textures_begin();
    structure_render_pass(render.environment_surface_indices, render.environment_surface_count,
                          structure_render_globals.environment_triangles_index,
                          nullptr, rasterizer_environment_diffuse_texture_draw, nullptr, nullptr);
    rasterizer_environment_diffuse_textures_end();
}
