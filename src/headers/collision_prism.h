/* collision_prism — a prism collision feature (104 bytes): a planar face extruded by `height`, with up to 8
 * 2D boundary points in the projection plane. Layout verbatim from the database. */
#pragma once

#include "real_plane3d.h"
#include "real_point2d.h"

typedef struct collision_prism
{
    int           object_index;            /* 0x00 */
    int           surface_index;           /* 0x04 */
    unsigned char flags;                   /* 0x08 */
    unsigned char breakable_surface_index; /* 0x09 */
    __int16       material_index;          /* 0x0A */
    real_plane3d  plane;                   /* 0x0C */
    float         height;                  /* 0x1C */
    __int16       projection_axis;         /* 0x20 */
    unsigned char projection_sign;         /* 0x22 */
    unsigned char _pad0;                    /* 0x23 padding byte */
    int           point_count;             /* 0x24 */
    real_point2d  points[8];               /* 0x28 */
} collision_prism;
