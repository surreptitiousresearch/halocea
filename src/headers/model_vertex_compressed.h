#pragma once
/* model_vertex_compressed — the packed (skinned) model render vertex (32 bytes). The float
 * position is stored uncompressed; normal/binormal/tangent are 11-11-10 packed dwords; the
 * texcoord is a signed 16-bit pair; nodes are byte palette indices (stored *3) and weights a
 * single quantised byte in the low half of weights[0]. Layout confirmed against the database
 * type of the same name. */

#include <stdint.h>
#include "real_point3d.h"

typedef struct model_vertex_compressed
{
    real_point3d  position;    /* 0x00 */
    unsigned int  normal;      /* 0x0C */
    unsigned int  binormal;    /* 0x10 */
    unsigned int  tangent;     /* 0x14 */
    int16_t       texcoord_u;  /* 0x18 */
    int16_t       texcoord_v;  /* 0x1A */
    uint8_t nodes[2];  /* 0x1C */
    int16_t       weights[1];  /* 0x1E */
} model_vertex_compressed;     /* 0x20 = 32 bytes */
