/* rasterizer_transparent_geometry_group_draw_internal @0x838129F4 — draw one transparent geometry group's
 * triangles, dispatching to the static/dynamic triangle-buffer x static/dynamic vertex-buffer variant
 * implied by which of triangle_buffer/vertex_buffers are present. With no triangle buffer and no vertex
 * buffer, dynamic_triangle_buffer_index doubles as either a real buffer index (>=0) or, when negative, an
 * encoded primitive type (-3/-4 = strip/fan, dividing triangle_count by (type-2) for the vertex count;
 * anything else = a single primitive of 1 vertex) passed straight to rasterizer_draw_dynamic_vertices.
 *
 * DEVIATION: the decompiler rendered the encoded-primitive-type extraction as `(__int16)-HIWORD(dynamic_
 * triangle_buffer_index)`; disasm (0x83812A60-0x83812A70) shows a plain `neg` followed by `extsh` (sign-
 * extend the LOW 16 bits, not the high word) — the real formula is simply `(__int16)(-dynamic_triangle_
 * buffer_index)`. */

#include <stdint.h>
#include "headers/transparent_geometry_group.h"

extern void rasterizer_draw_static_triangles_static_vertices(const triangle_buffer *triangle_buffer, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);
extern void rasterizer_draw_static_triangles_dynamic_vertices(const triangle_buffer *triangle_buffer, int first_triangle_index, int triangle_count, int dynamic_vertex_buffer_index);
extern void rasterizer_draw_dynamic_triangles_static_vertices(int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);
extern void rasterizer_draw_dynamic_triangles_static_vertices2(int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer0, const vertex_buffer *vertex_buffer1);
extern void rasterizer_draw_dynamic_triangles_dynamic_vertices(int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, int dynamic_vertex_buffer_index);
extern void rasterizer_draw_dynamic_vertices(int first_primitive_index, int primitive_count, int dynamic_vertex_buffer_index, int16_t vertices_per_primitive);

void rasterizer_transparent_geometry_group_draw_internal(const transparent_geometry_group *group, uint8_t has_lightmap)
{
    const vertex_buffer *vertex_buffers = group->vertex_buffers;
    const triangle_buffer *triangle_buffer = group->triangle_buffer;

    if ( triangle_buffer )
    {
        if ( vertex_buffers )
            rasterizer_draw_static_triangles_static_vertices(triangle_buffer, group->first_triangle_index,
                    group->triangle_count, vertex_buffers);
        else
            rasterizer_draw_static_triangles_dynamic_vertices(triangle_buffer, group->first_triangle_index,
                    group->triangle_count, group->dynamic_vertex_buffer_index);
        return;
    }

    int dynamic_triangle_buffer_index = group->dynamic_triangle_buffer_index;

    if ( vertex_buffers )
    {
        if ( has_lightmap )
            rasterizer_draw_dynamic_triangles_static_vertices2(dynamic_triangle_buffer_index,
                    group->first_triangle_index, group->triangle_count, vertex_buffers, vertex_buffers + 1);
        else
            rasterizer_draw_dynamic_triangles_static_vertices(dynamic_triangle_buffer_index,
                    group->first_triangle_index, group->triangle_count, vertex_buffers);
        return;
    }

    if ( dynamic_triangle_buffer_index >= 0 )
    {
        rasterizer_draw_dynamic_triangles_dynamic_vertices(dynamic_triangle_buffer_index,
                group->first_triangle_index, group->triangle_count, group->dynamic_vertex_buffer_index);
        return;
    }

    /* negative buffer index encodes vertices-per-primitive (3 = triangle list, 4 = quad list;
     * see rasterizer_draw_dynamic_vertices), not an enum */
    int16_t vertices_per_primitive = (int16_t)(-dynamic_triangle_buffer_index);
    int16_t primitive_count;

    if ( vertices_per_primitive == 3 || vertices_per_primitive == 4 )
        primitive_count = (int16_t)(group->triangle_count / (vertices_per_primitive - 2));
    else
        primitive_count = 1;

    rasterizer_draw_dynamic_vertices(0, primitive_count, group->dynamic_vertex_buffer_index, vertices_per_primitive);
}
