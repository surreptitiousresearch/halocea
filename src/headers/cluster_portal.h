#pragma once
/* cluster_portal — a portal between two structure clusters (DB-verified, 64 bytes). */

#include "real_point3d.h"
#include "tag_block.h"

typedef struct cluster_portal
{
    __int16 cluster_indices[2]; /* 0x00 */
    int plane_index;            /* 0x04 */
    real_point3d centroid;      /* 0x08 */
    float bounding_radius;      /* 0x14 */
    int flags;                  /* 0x18 */
    int unused[6];              /* 0x1C */
    tag_block vertices;         /* 0x34: real_point3d[] */
} cluster_portal;
