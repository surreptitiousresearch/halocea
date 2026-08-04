#pragma once
/* environment_lightmap_vertex_compressed — packed per-vertex lightmap record: incident radiosity plus
 * a 16-bit signed lightmap UV pair. get_texcoord maps each axis via (2*v + 1) / 65536. */

typedef struct environment_lightmap_vertex_compressed
{
    unsigned int incident_radiosity;  /* 0x00 */
    __int16      lightmap_u;          /* 0x04 */
    __int16      lightmap_v;          /* 0x06 */
} environment_lightmap_vertex_compressed;   /* 8 bytes */
