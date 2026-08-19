/* rasterizer_draw_dynamic_triangles_dynamic_vertices @0x837915E0 — submit a run of dynamic triangles over
 * a dynamic vertex buffer, in chunks of at most 10000 triangles per draw: bind the shared dynamic vertex
 * buffer at the buffer's offset/stride, set the dynamic index buffer and issue an indexed triangle-list
 * draw from the triangle buffer's index window. */

#include <stdint.h>
#include "headers/dynamic_vertices.h"
#include "headers/dynamic_triangles.h"
#include "headers/rasterizer_geometry_limits.h"
#include "headers/d3d_boundary.h"
#include "headers/_D3DPRIMITIVETYPE.h"
#include "headers/blam_data_globals.h"

extern dynamic_triangles_struct dynamic_triangles;

extern int16_t rasterizer_geometry_get_vertex_size(int16_t type);
extern void rasterizer_dx9_dynamic_vertex_buffer_set(unsigned int vertex_buffer_index, unsigned int stream_index, unsigned int offset, unsigned int stride);
extern void D3DDevice_SetIndices(D3DDevice *pDevice, D3DIndexBuffer *pIndexData);
extern void D3DDevice_DrawIndexedVertices(D3DDevice *pDevice, _D3DPRIMITIVETYPE PrimitiveType,
        unsigned int BaseVertexIndex, unsigned int StartIndex, unsigned int IndexCount);

void rasterizer_draw_dynamic_triangles_dynamic_vertices(int dynamic_triangle_buffer_index,
        int first_triangle_index, int triangle_count, int dynamic_vertex_buffer_index)
{
    for ( int remaining = triangle_count; remaining > 0; )
    {
        if ( dynamic_triangle_buffer_index == -1 || dynamic_vertex_buffer_index == -1 )
            break;

        dynamic_vertex_buffer *vertex_buffer = &dynamic_vertices.buffers[dynamic_vertex_buffer_index];
        unsigned int vertex_size = rasterizer_geometry_get_vertex_size(vertex_buffer->type);

        int chunk = RASTERIZER_MAXIMUM_PRIMITIVES_PER_DRAW_COMMAND;
        if ( remaining <= RASTERIZER_MAXIMUM_PRIMITIVES_PER_DRAW_COMMAND )
            chunk = remaining;

        rasterizer_dx9_dynamic_vertex_buffer_set(dynamic_vertices.groups[0].vertex_buffer_index, 0,
                                                 vertex_buffer->vertex_buf_offset, vertex_size);
        D3DDevice_SetIndices(global_d3d_device, dynamic_triangles.d3d_index_buffer);
        D3DDevice_DrawIndexedVertices(global_d3d_device, D3DPT_TRIANGLELIST,
                                      vertex_buffer->vertex_start_index,
                                      3 * first_triangle_index
                                    + dynamic_triangles.buffers[dynamic_triangle_buffer_index].triangle_start_ofs,
                                      3 * chunk);
        remaining -= chunk;
        first_triangle_index += chunk;
    }
}
