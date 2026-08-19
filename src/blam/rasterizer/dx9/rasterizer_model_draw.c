/* rasterizer_model_draw @0x836A6138 */
#include <stdint.h>
#include "headers/shader.h"
#include "headers/triangle_buffer.h"
#include "headers/vertex_buffer.h"

extern void _rasterizer_model_draw(const shader *shader_ref, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index);

void rasterizer_model_draw(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index)
{
    _rasterizer_model_draw(shader, shader_permutation_index, triangle_buffer,
                           dynamic_triangle_buffer_index, triangle_count,
                           vertex_buffer, dynamic_vertex_buffer_index);
}
