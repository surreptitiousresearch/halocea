/* get_face_vertex @0x837FF9D0 */
#include <stdint.h>
#include "headers/geosphere.h"

extern int16_t get_edge_vertex(geosphere *sphere, int16_t vertex_a, int16_t vertex_b, int16_t segment_index, int16_t *vertex_index, int16_t *vertex_subdivision_indices);
extern void calculate_vertex(geosphere *sphere, int16_t new_vertex, int16_t parent1, int16_t parent2, int16_t denominator, int16_t numerator);

/* return attested uint16_t: every return path zero-extends 16-bit (clrlwi r3,r3,16
   @0x837FFA58/A7C/AA4; lhzx @0x837FFB0C) — NOT extsh like sibling get_edge_vertex.
   corner0/corner2 fixed int/uint16_t -> int16_t: geosphere-family index params,
   forwarded unmodified into get_edge_vertex which extsh-normalizes them at entry. */
uint16_t get_face_vertex(geosphere *sphere, int16_t corner0, int16_t corner1, int16_t corner2, int16_t s1, int16_t s2,
                         int16_t *vertex_index, int16_t *vertex_subdivision_indices,
                         int16_t *vertex_face_indices)
{
    int      segment_count = sphere->segment_count;
    int16_t *slot = &vertex_face_indices[(int16_t)((segment_count + 1) * s1 + s2)];

    if ((uint16_t)*slot == 0xFFFF)
    {
        int16_t new_vertex;
        int16_t edge_a;
        int16_t edge_b;

        if (!s2)
        {
            *slot = get_edge_vertex(sphere, corner0, corner2, s1, vertex_index, vertex_subdivision_indices);
            return (uint16_t)*slot;
        }
        if (s1 == segment_count)
        {
            *slot = get_edge_vertex(sphere, corner2, corner1, s2, vertex_index, vertex_subdivision_indices);
            return (uint16_t)*slot;
        }
        if (s2 == s1)
        {
            *slot = get_edge_vertex(sphere, corner0, corner1, s1, vertex_index, vertex_subdivision_indices);
            return (uint16_t)*slot;
        }

        new_vertex = (*vertex_index)++;
        edge_a     = get_edge_vertex(sphere, corner0, corner2, s1, vertex_index, vertex_subdivision_indices);
        edge_b     = get_edge_vertex(sphere, corner0, corner1, s1, vertex_index, vertex_subdivision_indices);
        *slot      = new_vertex;
        calculate_vertex(sphere, new_vertex, edge_a, edge_b, s1, s2);
    }

    return (uint16_t)*slot;
}
