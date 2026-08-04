#pragma once
/* structure_weather_polyhedron — a weather exclusion volume in a structure BSP (32 bytes; DB-verified). */

#include "real_point3d.h"
#include "tag_block.h"

typedef struct structure_weather_polyhedron
{
    real_point3d bounding_sphere_center;    /* 0x00 */
    float        bounding_sphere_radius;    /* 0x0C */
    int          unused;                    /* 0x10 */
    tag_block    planes;                    /* 0x14 */
} structure_weather_polyhedron;             /* 32 bytes */
