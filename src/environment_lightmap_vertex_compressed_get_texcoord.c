/* environment_lightmap_vertex_compressed_get_texcoord @0x837E98F0 — decode the packed 16-bit lightmap
 * UV of a compressed lightmap vertex into a float texcoord: each axis = (2*v + 1) / 65536. */

#include "headers/environment_lightmap_vertex_compressed.h"
#include "headers/real_point2d.h"

void environment_lightmap_vertex_compressed_get_texcoord(
        const environment_lightmap_vertex_compressed *vertex, real_point2d *texcoord)
{
    texcoord->n[0] = (((float)vertex->lightmap_u * (float)2.0) + (float)1.0)
                   * (float)0.000015259022;
    texcoord->n[1] = (((float)vertex->lightmap_v * (float)2.0) + (float)1.0)
                   * (float)0.000015259022;
}
