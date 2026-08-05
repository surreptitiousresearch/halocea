/* collision_cylinder — a cylinder collision feature (40 bytes). Layout verbatim from the database. */
#pragma once

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct collision_cylinder
{
    int           object_index;            /* 0x00 */
    int           surface_index;           /* 0x04 */
    unsigned char flags;                   /* 0x08 */
    unsigned char breakable_surface_index; /* 0x09 */
    int16_t       material_index;          /* 0x0A */
    real_point3d  base;                    /* 0x0C */
    real_vector3d height;                  /* 0x18 */
    float         width;                   /* 0x24 */
} collision_cylinder;
