/* structure_render_specular_lightmaps @0x837C6A60 — render the specular-lightmap pass over the BSP
 * environment surfaces. Same lightmap_mode save/force/restore handling as structure_render_lightmaps.
 * No-op when the environment geometry is invalid. */

#include <stdint.h>
#include "headers/structure_render_globals.h"
#include "headers/render_globals.h"
#include "headers/structure_render_pass.h"
#include "headers/rasterizer_globals.h"
#include "headers/structure_bsp.h"

extern void rasterizer_environment_specular_lightmaps_begin(void);
extern void rasterizer_environment_specular_lightmaps_end(void);
extern void rasterizer_environment_specular_lightmap_begin(const struct bitmap_data *lightmap);
extern void rasterizer_environment_specular_lightmap_draw(const struct shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const struct vertex_buffer *vertex_buffer);
extern void rasterizer_environment_specular_lightmap_end(void);

void structure_render_specular_lightmaps(void)
{
    if (!structure_render_globals.environment_geometry_valid)
        return;

    __int16 saved_lightmap_mode = rasterizer_globals.lightmap_mode;
    if (global_structure_bsp->lightmap_group.index == -1 && !rasterizer_globals.lightmap_mode)
        rasterizer_globals.lightmap_mode = 1;

    rasterizer_environment_specular_lightmaps_begin();
    structure_render_pass(render.environment_surface_indices, render.environment_surface_count,
                          structure_render_globals.environment_triangles_index,
                          (void (*)(struct bitmap_data *))rasterizer_environment_specular_lightmap_begin,
                          rasterizer_environment_specular_lightmap_draw,
                          (void (*)(void))rasterizer_environment_specular_lightmap_end, nullptr);
    rasterizer_environment_specular_lightmaps_end();

    rasterizer_globals.lightmap_mode = saved_lightmap_mode;
}
