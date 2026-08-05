/* collision_result — outcome of a collision query (vector / point test): what was hit, where, the
 * surface plane, and the BSP locations at the start and impact points (80 bytes). type -1 = nothing
 * hit; 0 = fog plane, 2 = structure BSP surface; object hits fill object/region/node/bsp indices. */
#pragma once

#include <stdint.h>
#include "location.h"
#include "real_point3d.h"
#include "real_plane3d.h"

typedef struct collision_result
{
    int16_t       type;                    /* 0x00 */
    unsigned char _pad0[2]; /* db-verified padding */
    location      start_location;          /* 0x04 */
    location      location;                /* 0x0C */
    float         t;                       /* 0x14 — fraction along the test vector */
    real_point3d  point;                   /* 0x18 — impact point */
    real_plane3d  plane;                   /* 0x24 — surface plane (oriented toward the ray origin) */
    int16_t       material_type;           /* 0x34 */
    unsigned char _pad1[2]; /* db-verified padding */
    int           object_index;            /* 0x38 */
    int16_t       region_index;            /* 0x3C */
    int16_t       node_index;              /* 0x3E */
    int16_t       bsp_index;               /* 0x40 */
    unsigned char _pad2[2]; /* db-verified padding */
    int           surface_index;           /* 0x44 */
    int           plane_designator;        /* 0x48 */
    unsigned char flags;                   /* 0x4C */
    unsigned char breakable_surface_index; /* 0x4D */
    int16_t       material_index;          /* 0x4E */
} collision_result;
