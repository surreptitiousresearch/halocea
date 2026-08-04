/* collision_result — outcome of a collision query (vector / point test): what was hit, where, the
 * surface plane, and the BSP locations at the start and impact points (80 bytes). type -1 = nothing
 * hit; 0 = fog plane, 2 = structure BSP surface; object hits fill object/region/node/bsp indices. */
#pragma once

#include "location.h"
#include "real_point3d.h"
#include "real_plane3d.h"

typedef struct collision_result
{
    __int16       type;                    /* 0x00 */
    unsigned char _pad0[2]; /* db-verified padding */
    location      start_location;          /* 0x04 */
    location      location;                /* 0x0C */
    float         t;                       /* 0x14 — fraction along the test vector */
    real_point3d  point;                   /* 0x18 — impact point */
    real_plane3d  plane;                   /* 0x24 — surface plane (oriented toward the ray origin) */
    __int16       material_type;           /* 0x34 */
    unsigned char _pad1[2]; /* db-verified padding */
    int           object_index;            /* 0x38 */
    __int16       region_index;            /* 0x3C */
    __int16       node_index;              /* 0x3E */
    __int16       bsp_index;               /* 0x40 */
    unsigned char _pad2[2]; /* db-verified padding */
    int           surface_index;           /* 0x44 */
    int           plane_designator;        /* 0x48 */
    unsigned char flags;                   /* 0x4C */
    unsigned char breakable_surface_index; /* 0x4D */
    __int16       material_index;          /* 0x4E */
} collision_result;
