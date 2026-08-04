/* model_vertex_uncompressed — the full (uncompressed) rigid model render vertex (0x44 = 68 bytes).
 * Layout confirmed against the database type of the same name. */
#pragma once

#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_point2d.h"

typedef struct model_vertex_uncompressed
{
    real_point3d  position;    /* 0x00 */
    real_vector3d normal;      /* 0x0C */
    real_vector3d binormal;    /* 0x18 */
    real_vector3d tangent;     /* 0x24 */
    real_point2d  texcoord;    /* 0x30 */
    __int16       nodes[2];    /* 0x38 */
    float         weights[2];  /* 0x3C */
} model_vertex_uncompressed;   /* 0x44 */
