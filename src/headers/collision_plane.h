/* collision_plane — result of a collision-feature point/vector test: the contact fraction and point, the contact
 * plane, and the struck object/surface/material. Layout verbatim from the database. The walkable-slope test in
 * collision_fix_pill reads plane.normal.n[2] (the ground-normal up component). */
#pragma once

#include <stdint.h>
#include "real_point3d.h"
#include "real_plane3d.h"

typedef struct collision_plane
{
    float         t;                       /* 0x00 — fraction along the test/nudge vector */
    real_point3d  point;                   /* 0x04 — contact point */
    real_plane3d  plane;                   /* 0x10 — contact plane (normal.n[2] == slope) */
    int           object_index;            /* 0x20 */
    int           surface_index;           /* 0x24 */
    unsigned char flags;                   /* 0x28 */
    unsigned char breakable_surface_index; /* 0x29 */
    int16_t       material_index;          /* 0x2A */
} collision_plane;
