/* rasterizer_geometry_get_vertex_size 0x837E83F0 — look up the byte size of one vertex of the given vertex
 * type from the static size table. */

#include <stdint.h>

/* lhzx (halfword) load with stride-2 index proves int16_t elements; return path extsh -> int16_t. */
extern const int16_t rasterizer_vertex_type_sizes[];

int16_t rasterizer_geometry_get_vertex_size(int16_t type)
{
    return rasterizer_vertex_type_sizes[type];
}
