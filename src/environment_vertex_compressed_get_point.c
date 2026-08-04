/* environment_vertex_compressed_get_point @0x837E9748 — copy the position out of a compressed
 * structure render vertex. */

#include "headers/environment_vertex_compressed.h"
#include "headers/real_point3d.h"

void environment_vertex_compressed_get_point(const environment_vertex_compressed *vertex,
        real_point3d *point)
{
    *point = vertex->position;
}
