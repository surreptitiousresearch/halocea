/* collision_sphere — a sphere collision feature (28 bytes). Layout verbatim from the database. */
#pragma once

#include <stdint.h>
#include "real_point3d.h"

typedef struct collision_sphere
{
    int           object_index;            /* 0x00 */
    int           surface_index;           /* 0x04 */
    unsigned char flags;                   /* 0x08 */
    unsigned char breakable_surface_index; /* 0x09 */
    int16_t       material_index;          /* 0x0A */
    real_point3d  center;                  /* 0x0C */
    float         radius;                  /* 0x18 */
} collision_sphere;
