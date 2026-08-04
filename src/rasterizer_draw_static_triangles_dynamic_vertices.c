/* rasterizer_draw_static_triangles_dynamic_vertices @0x837918E0 — draw a run of triangles from a static
 * (device-resident) triangle_buffer's index window, using a dynamic vertex buffer, batching at most 10000
 * triangles per draw call. Hybrid of rasterizer_draw_static_triangles_static_vertices.c (static index
 * buffer / triangle_buffer bookkeeping) and rasterizer_draw_dynamic_triangles_dynamic_vertices.c (dynamic
 * vertex buffer binding). */

#include <stdint.h>
#include "headers/triangle_buffer.h"
#include "headers/triangle_buffer_type.h"
#include "headers/rasterizer_geometry_limits.h"
#include "headers/dynamic_vertices.h"
#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"


extern int16_t rasterizer_geometry_get_vertex_size(int16_t type);
extern void rasterizer_dx9_dynamic_vertex_buffer_set(unsigned int vertex_buffer_index, unsigned int stream_index, unsigned int offset, unsigned int stride);
extern D3DIndexBuffer *hcex_stat_ibuf_get(void *static_ibuf);
extern void D3DDevice_SetIndices(D3DDevice *pDevice, D3DIndexBuffer *pIndexData);
extern void D3DDevice_DrawIndexedVertices(D3DDevice *pDevice, _D3DPRIMITIVETYPE PrimitiveType,
        unsigned int BaseVertexIndex, unsigned int StartIndex, unsigned int IndexCount);

void rasterizer_draw_static_triangles_dynamic_vertices(const triangle_buffer *triangle_buffer,
    int first_triangle_index, int triangle_count, int dynamic_vertex_buffer_index)
{
    int base_index_offset = 0;

    while ( triangle_count > 0 )
    {
        if ( !triangle_buffer || !triangle_buffer->hardware_format || dynamic_vertex_buffer_index == -1 )
            break;

        dynamic_vertex_buffer *vertex_buffer = &dynamic_vertices.buffers[dynamic_vertex_buffer_index];
        unsigned int vertex_size = rasterizer_geometry_get_vertex_size(vertex_buffer->type);

        int batch_count = (triangle_count > RASTERIZER_MAXIMUM_PRIMITIVES_PER_DRAW_COMMAND)
                              ? RASTERIZER_MAXIMUM_PRIMITIVES_PER_DRAW_COMMAND : triangle_count;

        rasterizer_dx9_dynamic_vertex_buffer_set(dynamic_vertices.groups[0].vertex_buffer_index, 0,
            vertex_buffer->vertex_buf_offset, vertex_size);

        D3DIndexBuffer *hw_index_buffer = hcex_stat_ibuf_get(triangle_buffer->hardware_format);
        D3DDevice_SetIndices(global_d3d_device, hw_index_buffer);

        _D3DPRIMITIVETYPE primitive_type = d3d_primitive_type_table[triangle_buffer->type];
        D3DDevice_DrawIndexedVertices(global_d3d_device, primitive_type,
            vertex_buffer->vertex_start_index, ((unsigned int)triangle_buffer->offset >> 1) + base_index_offset,
            D3DPRIMITIVEVERTEXCOUNT[primitive_type][0] * batch_count + D3DPRIMITIVEVERTEXCOUNT[primitive_type][1]);

        triangle_count -= batch_count;

        if ( triangle_buffer->type == _triangle_buffer_type_triangles )
            base_index_offset += 3 * batch_count;
        else if ( triangle_buffer->type == _triangle_buffer_type_precompiled_strip )
            base_index_offset += batch_count;
    }
}
