/* rasterizer_draw_dynamic_vertices @0x83791B90 — draw a run of primitives straight out of a dynamic vertex
 * buffer (no index buffer), batching at most 10000 primitives per draw call. The primitive type is chosen
 * from vertices_per_primitive: 2 -> line list, 3 -> triangle list, anything else -> triangle strip (with the
 * primitive count adjusted to verts-2). The special case 4 (quad list) has no native D3D primitive, so it
 * builds two triangles per quad into a temporary dynamic triangle buffer and defers to
 * rasterizer_draw_dynamic_triangles_dynamic_vertices. Sibling of
 * rasterizer_draw_static_triangles_dynamic_vertices.c. */

#include <stdint.h>
#include "headers/dynamic_vertices.h"
#include "headers/rasterizer_triangle.h"
#include "headers/rasterizer_geometry_limits.h"
#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"
#include "headers/geometry_constants.h"


extern int rasterizer_dynamic_triangles_new(int count);
extern rasterizer_triangle *rasterizer_dynamic_triangles_lock(int dynamic_triangle_buffer_index);
extern void rasterizer_dynamic_triangles_unlock(int dynamic_triangle_buffer_index);
extern void rasterizer_dynamic_triangles_delete(int dynamic_triangle_buffer_index);
extern void rasterizer_draw_dynamic_triangles_dynamic_vertices(int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, int dynamic_vertex_buffer_index);
extern int16_t rasterizer_geometry_get_vertex_size(int16_t type);
extern void rasterizer_dx9_dynamic_vertex_buffer_set(unsigned int vertex_buffer_index, unsigned int stream_index, unsigned int offset, unsigned int stride);
extern void D3DDevice_DrawVertices(D3DDevice *pDevice, _D3DPRIMITIVETYPE PrimitiveType,
        unsigned int StartVertex, unsigned int VertexCount);

void rasterizer_draw_dynamic_vertices(int first_primitive_index, int primitive_count,
        int dynamic_vertex_buffer_index, int16_t vertices_per_primitive)
{
    int remaining = primitive_count;
    if ( primitive_count <= 0 )
        return;

    while ( dynamic_vertex_buffer_index != -1 )
    {
        _D3DPRIMITIVETYPE primitive_type;

        switch ( vertices_per_primitive )
        {
            case NUMBER_OF_VERTICES_PER_LINE:
                primitive_type = D3DPT_LINELIST;
                break;

            case NUMBER_OF_VERTICES_PER_TRIANGLE:
                primitive_type = D3DPT_TRIANGLELIST;
                break;

            case NUMBER_OF_VERTICES_PER_QUADRILATERAL:
            {
                int triangle_count = 2 * remaining;
                int triangle_buffer_index = rasterizer_dynamic_triangles_new(triangle_count);
                if ( triangle_buffer_index != -1 )
                {
                    rasterizer_triangle *triangles = rasterizer_dynamic_triangles_lock(triangle_buffer_index);
                    for ( int i = 0; i < triangle_count; i = (int16_t)(i + 2) )
                    {
                        int base = 4 * (i / 2);
                        triangles[i].vertex_indices[0] = base;
                        triangles[i].vertex_indices[1] = base + 1;
                        triangles[i].vertex_indices[2] = base + 2;
                        triangles[i + 1].vertex_indices[0] = base;
                        triangles[i + 1].vertex_indices[1] = base + 2;
                        triangles[i + 1].vertex_indices[2] = base + 3;
                    }
                    rasterizer_dynamic_triangles_unlock(triangle_buffer_index);
                    rasterizer_draw_dynamic_triangles_dynamic_vertices(triangle_buffer_index, 0, triangle_count,
                        dynamic_vertex_buffer_index);
                    rasterizer_dynamic_triangles_delete(triangle_buffer_index);
                }
                return;
            }

            default:
                remaining = vertices_per_primitive - 2;
                primitive_type = D3DPT_TRIANGLESTRIP;
                break;
        }

        dynamic_vertex_buffer *vertex_buffer = &dynamic_vertices.buffers[dynamic_vertex_buffer_index];
        unsigned int vertex_size = rasterizer_geometry_get_vertex_size(vertex_buffer->type);

        int chunk = (remaining > RASTERIZER_MAXIMUM_PRIMITIVES_PER_DRAW_COMMAND)
                        ? RASTERIZER_MAXIMUM_PRIMITIVES_PER_DRAW_COMMAND : remaining;

        rasterizer_dx9_dynamic_vertex_buffer_set(dynamic_vertices.groups[0].vertex_buffer_index, 0,
            vertex_buffer->vertex_buf_offset, vertex_size);
        D3DDevice_DrawVertices(global_d3d_device, primitive_type,
            vertices_per_primitive * first_primitive_index + vertex_buffer->vertex_start_index,
            D3DPRIMITIVEVERTEXCOUNT[primitive_type][0] * chunk + D3DPRIMITIVEVERTEXCOUNT[primitive_type][1]);

        remaining -= chunk;
        first_primitive_index += chunk;
        if ( remaining <= 0 )
            return;
    }
}
