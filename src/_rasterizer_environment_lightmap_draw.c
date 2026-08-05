#include <stdint.h>
/* _rasterizer_environment_lightmap_draw @0x8378F5D0 — forward to the active lightmap draw function for the
 * environment surface batch. */

struct shader;
struct vertex_buffer;

/* funcptr GLOBAL @0x8465E5DC (lwz+mtctr+bctr @0x8378F5D0), defined in src/data — was wrongly
 * re-declared as an extern function here (would not link against the data symbol) */
extern void (*rasterizer_environment_lightmaps_draw_function)(
    const struct shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index,
    int first_triangle_index, int triangle_count, const struct vertex_buffer *vertex_buffer);

void _rasterizer_environment_lightmap_draw(const struct shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const struct vertex_buffer *vertex_buffer)
{
    rasterizer_environment_lightmaps_draw_function(shader, shader_permutation_index,
                                                   dynamic_triangle_buffer_index, first_triangle_index,
                                                   triangle_count, vertex_buffer);
}
