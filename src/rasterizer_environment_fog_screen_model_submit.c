/* rasterizer_environment_fog_screen_model_submit @0x8378FFC0 — no-op (bare blr; confirmed via disasm). */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/triangle_buffer.h"
#include "headers/vertex_buffer.h"

void rasterizer_environment_fog_screen_model_submit(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index)
{
}
