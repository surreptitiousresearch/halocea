#include <stdint.h>
#include "headers/geosphere.h"

extern void calculate_vertex(geosphere *sphere, int16_t new_vertex, int16_t parent1, int16_t parent2, int16_t denominator, int16_t numerator);

/* params/return attested int16_t: callee re-extends v1/v2/segment_index with
   extsh @0x837FF77C-837FF7BC and extsh's the composed result before blr
   @0x837FF8A4; early exits return the raw halfword args. */
int16_t get_edge_vertex(geosphere *sphere, int16_t vertex_a, int16_t vertex_b, int16_t segment_index,
                        int16_t *vertex_index, int16_t *vertex_subdivision_indices)
{
    int16_t low      = (vertex_a <= vertex_b) ? vertex_a : vertex_b;
    int16_t high     = (vertex_a <= vertex_b) ? vertex_b : vertex_a;
    /* faithful subfc/eqv carry decode: unsigned compare of the sign-extended values */
    char    ascending = (((vertex_a ^ vertex_b) >= 0)
                       + ((unsigned int)vertex_b >= (unsigned int)vertex_a)) & 1;
    int     segment_count;
    int     edge_index;
    int     offset; /* stays 32-bit in the binary; only the final sum is extsh'd */

    if (!segment_index)
        return vertex_a;

    segment_count = sphere->segment_count;
    if (segment_index == segment_count)
        return vertex_b;

    edge_index = 8 * low + high;
    if ((uint16_t)vertex_subdivision_indices[edge_index] == 0xFFFF)
    {
        int16_t i;
        vertex_subdivision_indices[edge_index] = *vertex_index;
        segment_count = sphere->segment_count;
        for (i = 1; i < segment_count; i = (int16_t)(i + 1))
        {
            int16_t new_vertex = (*vertex_index)++;
            calculate_vertex(sphere, new_vertex, low, high, sphere->segment_count, i);
            segment_count = sphere->segment_count;
        }
    }

    offset = ascending ? (segment_count - segment_index) : segment_index;
    return (int16_t)(vertex_subdivision_indices[edge_index] + offset - 1);
}
