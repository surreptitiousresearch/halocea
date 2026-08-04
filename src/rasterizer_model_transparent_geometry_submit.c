/* rasterizer_model_transparent_geometry_submit @0x836A6140 — thin forwarding wrapper: reshuffles the
 * 9th (stack-passed) argument into its own frame before tail-calling the real implementation,
 * `_rasterizer_model_transparent_geometry_submit` (1124B @0x8378CD18, see _rasterizer_model_transparent_geometry_submit.c —
 * the address recorded here previously, 0x836EC098, was a stale/mis-copied hex conversion that actually
 * falls inside an unrelated function; corrected via a fresh `funcs` lookup by name). */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/triangle_buffer.h"
#include "headers/vertex_buffer.h"
#include "headers/real_point3d.h"
#include "headers/render_sort_filth.h"
#include "headers/transparent_geometry_group.h"

#include "headers/transparent_geometry_group.h"
extern transparent_geometry_group *_rasterizer_model_transparent_geometry_submit(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index, const real_point3d *centroid, render_sort_filth *sort_filth);

void rasterizer_model_transparent_geometry_submit(
        const shader *shader,
        int16_t shader_permutation_index,
        const triangle_buffer *triangle_buffer,
        int dynamic_triangle_buffer_index,
        int triangle_count,
        const vertex_buffer *vertex_buffer,
        int dynamic_vertex_buffer_index,
        const real_point3d *centroid,
        render_sort_filth *sort_filth)
{
    _rasterizer_model_transparent_geometry_submit(shader, shader_permutation_index, triangle_buffer,
            dynamic_triangle_buffer_index, triangle_count, vertex_buffer, dynamic_vertex_buffer_index,
            centroid, sort_filth);
}
