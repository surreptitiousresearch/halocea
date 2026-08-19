/* rasterizer_environment_diffuse_light_draw @0x836A61B8 */
/* rasterizer_environment_diffuse_light_draw 0x836A61B8 (thunk) — tail-call thunk to the underscore implementation. */
#include <stdint.h>

struct shader;
struct vertex_buffer;

extern void _rasterizer_environment_diffuse_light_draw(const struct shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const struct vertex_buffer *vertex_buffer);

void rasterizer_environment_diffuse_light_draw(const struct shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const struct vertex_buffer *vertex_buffer)
{
    _rasterizer_environment_diffuse_light_draw(shader, shader_permutation_index, dynamic_triangle_buffer_index, first_triangle_index,
           triangle_count, vertex_buffer);
}
