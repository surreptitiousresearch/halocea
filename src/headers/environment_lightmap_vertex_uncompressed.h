#pragma once
/* environment_lightmap_vertex_uncompressed — the decoded per-vertex lightmap record: incident
 * radiosity as a float vector plus a float lightmap texcoord. Layout confirmed against the
 * database type of the same name (20 bytes). */

#include "real_vector3d.h"
#include "real_point2d.h"

typedef struct environment_lightmap_vertex_uncompressed
{
    real_vector3d incident_radiosity;  /* 0x00 */
    real_point2d  texcoord;            /* 0x0C */
} environment_lightmap_vertex_uncompressed;   /* 0x14 = 20 bytes */
