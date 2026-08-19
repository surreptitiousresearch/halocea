/* rasterizer_draw_static_triangles_static_vertices @0x83791A10 — draw a run of triangles from a static
 * (device-resident) triangle_buffer/vertex_buffer pair, batching at most 10000 triangles per draw call.
 *
 * DEVIATION: `first_triangle_index` is a genuinely dead parameter — traced in disasm, r4 is never saved or
 * read anywhere in the function body; the running index base always starts at 0.
 * DEVIATION: the decompiler's PendingMask3 for D3DDevice_SetStreamSource is garbage (LODWORD of an
 * uninitialized 64-bit local); disasm shows it is the literal constant 1 (`li r8, 1`), not derived from
 * triangle_buffer->offset (that value is `>> 1`'d separately into the DrawIndexedVertices BaseVertexIndex). */

#include <stdint.h>
#include "headers/triangle_buffer.h"
#include "headers/triangle_buffer_type.h"
#include "headers/rasterizer_geometry_limits.h"
#include "headers/vertex_buffer.h"
#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"


extern int16_t rasterizer_geometry_get_vertex_size(int16_t type);
extern D3DVertexBuffer *hcex_stat_vbuf_get(void *static_vbuf);
extern D3DIndexBuffer  *hcex_stat_ibuf_get(void *static_vbuf);

void rasterizer_draw_static_triangles_static_vertices(const triangle_buffer *triangle_buffer,
                                                       int first_triangle_index, int triangle_count,
                                                       const vertex_buffer *vertex_buffer)
{
    int base_vertex_offset = 0;

    while (triangle_count > 0)
    {
        if (!triangle_buffer || !triangle_buffer->hardware_format
            || !vertex_buffer || !vertex_buffer->hardware_format)
            break;

        int vertex_size = rasterizer_geometry_get_vertex_size(vertex_buffer->type);

        int batch_count = (triangle_count > RASTERIZER_MAXIMUM_PRIMITIVES_PER_DRAW_COMMAND)
                              ? RASTERIZER_MAXIMUM_PRIMITIVES_PER_DRAW_COMMAND : triangle_count;

        D3DVertexBuffer *hw_vertex_buffer = hcex_stat_vbuf_get(vertex_buffer->hardware_format);
        D3DIndexBuffer  *hw_index_buffer  = hcex_stat_ibuf_get(triangle_buffer->hardware_format);

        D3DDevice_SetStreamSource(global_d3d_device, 0, hw_vertex_buffer, vertex_buffer->offset,
                                  vertex_size, 1);
        D3DDevice_SetIndices(global_d3d_device, hw_index_buffer);

        _D3DPRIMITIVETYPE primitive_type = d3d_primitive_type_table[triangle_buffer->type];
        D3DDevice_DrawIndexedVertices(global_d3d_device, primitive_type,
            0, (triangle_buffer->offset >> 1) + base_vertex_offset,
            D3DPRIMITIVEVERTEXCOUNT[primitive_type][0] * batch_count + D3DPRIMITIVEVERTEXCOUNT[primitive_type][1]);

        triangle_count -= batch_count;

        if (triangle_buffer->type == _triangle_buffer_type_triangles)
            base_vertex_offset += 3 * batch_count;
        else if (triangle_buffer->type == _triangle_buffer_type_precompiled_strip)
            base_vertex_offset += batch_count;
    }
}
