/* subdivide_triangle @0x837FFB18 */
#include <stdint.h>
#include "headers/geosphere.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
extern uint16_t get_face_vertex(geosphere *sphere, int16_t corner0, int16_t corner1, int16_t corner2, int16_t s1, int16_t s2, int16_t *vertex_index, int16_t *vertex_subdivision_indices, int16_t *vertex_face_indices);

void subdivide_triangle(geosphere *sphere, int16_t corner_vertex0, int16_t corner_vertex1, int16_t corner_vertex2,
                        int16_t *vertex_index, int16_t *triangle_strip_vertex_indices_index,
                        int16_t *vertex_subdivision_indices)
{
    int      face_count = (int16_t)((sphere->segment_count + 1) * (sphere->segment_count + 1));
    int16_t *vertex_face_indices;
    int16_t  i;
    int16_t  row;

    vertex_face_indices = dlMalloc(2 * face_count,
                              "D:\\Projects\\code\\HCEX\\sources\\math\\geometry.c", 0x9Au);
    if (!vertex_face_indices)
        return;

    for (i = 0; i < face_count; i = (int16_t)(i + 1))
        vertex_face_indices[i] = -1;

    for (row = 1; row <= sphere->segment_count; row = (int16_t)(row + 1))
    {
        int16_t col;

        sphere->triangle_strip_vertex_indices[(*triangle_strip_vertex_indices_index)++] = 2 * row + 1;
        ++sphere->triangle_strip_count;

        for (col = 1; col <= row; col = (int16_t)(col + 1))
        {
            int16_t fv_lower_left = get_face_vertex(sphere, corner_vertex0, corner_vertex1, corner_vertex2, row - 1, col - 1,
                                        vertex_index, vertex_subdivision_indices, vertex_face_indices);
            int16_t fv_upper_left = get_face_vertex(sphere, corner_vertex0, corner_vertex1, corner_vertex2, row, col - 1,
                                        vertex_index, vertex_subdivision_indices, vertex_face_indices);
            int16_t fv_upper_right = get_face_vertex(sphere, corner_vertex0, corner_vertex1, corner_vertex2, row, col,
                                        vertex_index, vertex_subdivision_indices, vertex_face_indices);

            if (col == 1)
            {
                sphere->triangle_strip_vertex_indices[(*triangle_strip_vertex_indices_index)++] = fv_upper_left;
                sphere->triangle_strip_vertex_indices[(*triangle_strip_vertex_indices_index)++] = fv_lower_left;
            }
            sphere->triangle_strip_vertex_indices[(*triangle_strip_vertex_indices_index)++] = fv_upper_right;

            if (col < row)
                sphere->triangle_strip_vertex_indices[(*triangle_strip_vertex_indices_index)++] =
                    get_face_vertex(sphere, corner_vertex0, corner_vertex1, corner_vertex2, row - 1, col,
                        vertex_index, vertex_subdivision_indices, vertex_face_indices);
        }
    }

    dlFree(vertex_face_indices);
}
