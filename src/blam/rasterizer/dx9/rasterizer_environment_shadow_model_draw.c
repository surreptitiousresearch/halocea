/* rasterizer_environment_shadow_model_draw @0x836A61E8 */
#include <stdint.h>
#include "headers/shader.h"
#include "headers/triangle_buffer.h"
#include "headers/vertex_buffer.h"

extern void _rasterizer_environment_shadow_model_draw(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, const vertex_buffer *vertex_buffer);

void rasterizer_environment_shadow_model_draw(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, const vertex_buffer *vertex_buffer)
{
    _rasterizer_environment_shadow_model_draw(shader, shader_permutation_index, triangle_buffer, vertex_buffer);
}
