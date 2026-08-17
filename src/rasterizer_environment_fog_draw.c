/* rasterizer_environment_fog_draw @0x836A6320 */
#include <stdint.h>
/* rasterizer_environment_fog_draw 0x836A6320 (thunk) — tail-call thunk to the underscore rasterizer implementation. */
struct shader;
struct vertex_buffer;

#include "headers/shader.h"
#include "headers/vertex_buffer.h"
extern void _rasterizer_environment_fog_draw(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);

void rasterizer_environment_fog_draw(const struct shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const struct vertex_buffer *vertex_buffer)
{
    _rasterizer_environment_fog_draw(shader, shader_permutation_index, dynamic_triangle_buffer_index, first_triangle_index, triangle_count, vertex_buffer);
}
