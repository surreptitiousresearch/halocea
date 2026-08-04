/* rasterizer_draw_dynamic_triangles_static_vertices 0x837916C0 — draw a run of dynamic (per-frame) indexed
 * triangles against a single static vertex stream. The triangle run is split into batches of at most 10000
 * triangles per draw call. Bails out immediately if the buffer index is invalid or the vertex stream has no
 * loaded hardware format.
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

void rasterizer_draw_dynamic_triangles_static_vertices(int dynamic_triangle_buffer_index, int first_triangle_index,
        int triangle_count, const vertex_buffer *vertex_buffer)
{
    int remaining;
    unsigned int vertex_size;
    int batch_triangles;
    D3DVertexBuffer *d3d_vertex_buffer;

    for ( remaining = triangle_count; remaining > 0; first_triangle_index += batch_triangles )
    {
        if ( dynamic_triangle_buffer_index == -1 || !vertex_buffer || !vertex_buffer->hardware_format )
            break;

        vertex_size = rasterizer_geometry_get_vertex_size(vertex_buffer->type);
        batch_triangles = RASTERIZER_MAXIMUM_PRIMITIVES_PER_DRAW_COMMAND;
        if ( remaining <= RASTERIZER_MAXIMUM_PRIMITIVES_PER_DRAW_COMMAND )
            batch_triangles = remaining;

        d3d_vertex_buffer = hcex_stat_vbuf_get(vertex_buffer->hardware_format);
        D3DDevice_SetStreamSource(global_d3d_device, 0, d3d_vertex_buffer, vertex_buffer->offset, vertex_size, 0);
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
