#pragma once
/* vehicle_avoidance_cylinder — one collected obstacle cylinder for the AI vector-avoidance pass
 * (DB layout, 24 bytes): the source object plus the cylinder's base point and dimensions. */

#include "real_point3d.h"

typedef struct vehicle_avoidance_cylinder
{
    int          object_index;  /* 0x00 */
    real_point3d base;          /* 0x04 */
    float        height;        /* 0x10 */
    float        width;         /* 0x14 */
} vehicle_avoidance_cylinder;   /* 24 bytes */
