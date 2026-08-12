#include <stdint.h>
#include "headers/geosphere.h"
#include "headers/geosphere_primitive.h"
#include "headers/real_point3d.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
extern void subdivide_triangle(geosphere *sphere, int16_t corner_vertex0, int16_t corner_vertex1, int16_t corner_vertex2, int16_t *vertex_index, int16_t *triangle_strip_vertex_indices_index, int16_t *vertex_subdivision_indices);

extern const real_point3d geosphere_primitive_vertices[6];
extern const int16_t      geosphere_primitive_triangles[8][3];

geosphere *geosphere_new(int16_t segment_count)
{
    geosphere *sphere;
    int        vertex_count;
    int16_t   *vertex_subdivision_indices;
    int16_t    vertex_index;
    int16_t    triangle_strip_index;
    int        i;

    sphere = dlMalloc(0x14u,
                 "D:\\Projects\\code\\HCEX\\sources\\math\\geometry.c", 0x3Au);
    if (!sphere)
        return sphere;

    sphere->segment_count  = segment_count;
    sphere->triangle_count = GEOSPHERE_PRIMITIVE_TRIANGLE_COUNT * segment_count * segment_count;
    vertex_count = ((8 * (segment_count - 2) * (segment_count - 1)) >> 1) + 12 * segment_count - 6;
    sphere->vertex_count = vertex_count;

    sphere->vertices = dlMalloc(12 * (int16_t)vertex_count,
                           "D:\\Projects\\code\\HCEX\\sources\\math\\geometry.c", 0x42u);
    sphere->triangle_strip_vertex_indices = dlMalloc(8 * sphere->triangle_count,
                           "D:\\Projects\\code\\HCEX\\sources\\math\\geometry.c", 0x43u);
    sphere->triangle_strip_count = 0;
    vertex_subdivision_indices = dlMalloc(0x80u,
                           "D:\\Projects\\code\\HCEX\\sources\\math\\geometry.c", 0x45u);

    if (!sphere->vertices
        || !sphere->triangle_strip_vertex_indices
        || !vertex_subdivision_indices)
    {
        if (sphere->vertices)
            dlFree(sphere->vertices);
        if (sphere->triangle_strip_vertex_indices)
            dlFree(sphere->triangle_strip_vertex_indices);
        if (vertex_subdivision_indices)
            dlFree(vertex_subdivision_indices);
        return sphere;
    }

    for (i = 0; i < 64; i++)
        vertex_subdivision_indices[i] = -1;

    for (vertex_index = 0; vertex_index < 6; vertex_index = (int16_t)(vertex_index + 1))
    {
        sphere->vertices[vertex_index].n[0] = geosphere_primitive_vertices[vertex_index].n[0];
        sphere->vertices[vertex_index].n[1] = geosphere_primitive_vertices[vertex_index].n[1];
        sphere->vertices[vertex_index].n[2] = geosphere_primitive_vertices[vertex_index].n[2];
    }

    triangle_strip_index = 0;
    for (i = 0; i < 8; i = (int16_t)(i + 1))
    {
        subdivide_triangle(sphere,
            geosphere_primitive_triangles[i][0],
            geosphere_primitive_triangles[i][1],
            geosphere_primitive_triangles[i][2],
            &vertex_index, &triangle_strip_index, vertex_subdivision_indices);
    }

    dlFree(vertex_subdivision_indices);
    return sphere;
}
