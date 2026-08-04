/* environment_vertex_compressed_get_texcoord @0x837E9820 — copy the diffuse texcoord out of a
 * compressed structure render vertex. */

#include "headers/environment_vertex_compressed.h"
#include "headers/real_point2d.h"

void environment_vertex_compressed_get_texcoord(const environment_vertex_compressed *vertex,
        real_point2d *texcoord)
{
    *texcoord = vertex->texcoord;
}
