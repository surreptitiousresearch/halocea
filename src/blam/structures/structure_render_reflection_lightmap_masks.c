/* structure_render_reflection_lightmap_masks @0x837C6B10 — render the reflection lightmap-mask pass over
 * the BSP environment surfaces. No-op when the environment geometry is invalid. */

#include <stdint.h>
#include "headers/structure_render_globals.h"
#include "headers/render_globals.h"
#include "headers/structure_render_pass.h"

extern void rasterizer_environment_reflection_lightmap_masks_begin(void);
extern void rasterizer_environment_reflection_lightmap_masks_end(void);
extern void rasterizer_environment_reflection_lightmap_mask_begin(const struct bitmap_data *lightmap);
extern void rasterizer_environment_reflection_lightmap_mask_draw(const struct shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const struct vertex_buffer *vertex_buffer);
extern void rasterizer_environment_reflection_lightmap_mask_end(void);

void structure_render_reflection_lightmap_masks(void)
{
    if (!structure_render_globals.environment_geometry_valid)
        return;

    rasterizer_environment_reflection_lightmap_masks_begin();
    structure_render_pass(render.environment_surface_indices, render.environment_surface_count,
                          structure_render_globals.environment_triangles_index,
                          (void (*)(struct bitmap_data *))rasterizer_environment_reflection_lightmap_mask_begin,
                          rasterizer_environment_reflection_lightmap_mask_draw,
                          (void (*)(void))rasterizer_environment_reflection_lightmap_mask_end, nullptr);
    rasterizer_environment_reflection_lightmap_masks_end();
}
