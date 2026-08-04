/* rasterizer_environment_shadow_draw 0x836A61F8 — tail-call thunk to _rasterizer_environment_shadow_draw. */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/vertex_buffer.h"

extern void _rasterizer_environment_shadow_draw(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);

void rasterizer_environment_shadow_draw(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer)
{
    _rasterizer_environment_shadow_draw(shader, shader_permutation_index, dynamic_triangle_buffer_index,
                                        first_triangle_index, triangle_count, vertex_buffer);
}
