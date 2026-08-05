#include <stdint.h>
/* _rasterizer_environment_diffuse_light_draw 0x837897D0 (thunk) — tail-call thunk to the active
 * diffuse-light draw function pointer (selected at pass-begin time by shader/lighting permutation). */

struct shader;
struct vertex_buffer;

/* funcptr GLOBAL @0x8465E600 (lwz+bctr in the thunk), defined in src/data — was wrongly
 * re-declared as an extern function here (would not link against the data symbol) */
extern void (*rasterizer_environment_diffuse_light_draw_function)(const struct shader *shader,
                   int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index,
                   int triangle_count, const struct vertex_buffer *vertex_buffer);

void _rasterizer_environment_diffuse_light_draw(const struct shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const struct vertex_buffer *vertex_buffer)
{
    rasterizer_environment_diffuse_light_draw_function(shader, shader_permutation_index, dynamic_triangle_buffer_index,
           first_triangle_index, triangle_count, vertex_buffer);
}
