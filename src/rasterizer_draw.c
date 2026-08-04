#include "headers/triangle_buffer.h"
#include "headers/vertex_buffer.h"

extern void rasterizer_draw_static_triangles_static_vertices(const triangle_buffer *triangle_buffer, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);
extern void rasterizer_draw_static_triangles_dynamic_vertices(const triangle_buffer *triangle_buffer, int first_triangle_index, int triangle_count, int dynamic_vertex_buffer_index);
extern void rasterizer_draw_dynamic_triangles_static_vertices(int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);
extern void rasterizer_draw_dynamic_triangles_dynamic_vertices(int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, int dynamic_vertex_buffer_index);

void rasterizer_draw(const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index,
        int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer,
        int dynamic_vertex_buffer_index)
{
    if (triangle_buffer)
    {
        if (vertex_buffer)
            rasterizer_draw_static_triangles_static_vertices(triangle_buffer, first_triangle_index,
                triangle_count, vertex_buffer);
        else
            rasterizer_draw_static_triangles_dynamic_vertices(triangle_buffer, first_triangle_index,
                triangle_count, dynamic_vertex_buffer_index);
    }
    else if (vertex_buffer)
    {
        rasterizer_draw_dynamic_triangles_static_vertices(dynamic_triangle_buffer_index,
            first_triangle_index, triangle_count, vertex_buffer);
    }
    else
    {
        rasterizer_draw_dynamic_triangles_dynamic_vertices(dynamic_triangle_buffer_index,
            first_triangle_index, triangle_count, dynamic_vertex_buffer_index);
    }
}
