/* rasterizer_draw_dynamic_triangles_static_vertices2 @0x837917A8 */
/* rasterizer_draw_dynamic_triangles_static_vertices2 0x837917A8 — like
 * rasterizer_draw_dynamic_triangles_static_vertices but binds two static vertex streams (stream 0 and 1)
 * before each draw. Used by shaders that read a second per-vertex stream (e.g. lightmap UVs). The triangle
 * run is split into batches of at most 10000 triangles; bails if either stream lacks a hardware format.
 *
 * DEVIATION: the Xbox-360 D3DDevice_SetStreamSource trailing PendingMask3 was an unresolved scratch register
 * (r10) in the decompiler; passed as 0 here (the real inlined call computes the GPU pending mask internally). */

#include <stdint.h>
#include "headers/vertex_buffer.h"
#include "headers/dynamic_triangles.h"
#include "headers/rasterizer_engine_globals.h"
#include "headers/rasterizer_geometry_limits.h"
#include "headers/d3d_render_boundary.h"

extern int16_t rasterizer_geometry_get_vertex_size(int16_t type);
extern D3DVertexBuffer *hcex_stat_vbuf_get(void *hardware_format);

void rasterizer_draw_dynamic_triangles_static_vertices2(int dynamic_triangle_buffer_index, int first_triangle_index,
        int triangle_count, const vertex_buffer *vertex_buffer0, const vertex_buffer *vertex_buffer1)
{
    int remaining;
    unsigned int vertex_size0;
    unsigned int vertex_size1;
    int batch_triangles;
    D3DVertexBuffer *d3d_vertex_buffer0;
    D3DVertexBuffer *d3d_vertex_buffer1;

    for ( remaining = triangle_count; remaining > 0; first_triangle_index += batch_triangles )
    {
        if ( dynamic_triangle_buffer_index == -1
          || !vertex_buffer0
          || !vertex_buffer0->hardware_format
          || !vertex_buffer1
          || !vertex_buffer1->hardware_format )
        {
            break;
        }

        vertex_size0 = rasterizer_geometry_get_vertex_size(vertex_buffer0->type);
        vertex_size1 = rasterizer_geometry_get_vertex_size(vertex_buffer1->type);
        batch_triangles = RASTERIZER_MAXIMUM_PRIMITIVES_PER_DRAW_COMMAND;
        if ( remaining <= RASTERIZER_MAXIMUM_PRIMITIVES_PER_DRAW_COMMAND )
            batch_triangles = remaining;

        d3d_vertex_buffer0 = hcex_stat_vbuf_get(vertex_buffer0->hardware_format);
        d3d_vertex_buffer1 = hcex_stat_vbuf_get(vertex_buffer1->hardware_format);
        D3DDevice_SetStreamSource(global_d3d_device, 0, d3d_vertex_buffer0, vertex_buffer0->offset, vertex_size0, 0);
        D3DDevice_SetStreamSource(global_d3d_device, 1, d3d_vertex_buffer1, vertex_buffer1->offset, vertex_size1, 0);
        D3DDevice_SetIndices(global_d3d_device, dynamic_triangles.d3d_index_buffer);
        D3DDevice_DrawIndexedVertices(
            global_d3d_device,
            D3DPT_TRIANGLELIST,
            0,
            3 * first_triangle_index + dynamic_triangles.buffers[dynamic_triangle_buffer_index].triangle_start_ofs,
            3 * batch_triangles);
        remaining -= batch_triangles;
    }
}
