#pragma once
/* environment_vertex_compressed — packed structure render vertex (position + compressed basis +
 * diffuse texcoord). Only `texcoord` is read by the texture sampling path. */

#include "real_point3d.h"
#include "real_point2d.h"

typedef struct environment_vertex_compressed
{
    real_point3d position;    /* 0x00 */
    unsigned int normal;      /* 0x0C */
    unsigned int binormal;    /* 0x10 */
    unsigned int tangent;     /* 0x14 */
    real_point2d texcoord;    /* 0x18 */
} environment_vertex_compressed;   /* 32 bytes */
