#pragma once
/* vector_avoidance_data — working set for the AI vector-avoidance pass (DB layout, 24648 bytes): the BSP
 * being tested, the mover's local frame (origin/forward/left/up), the collected obstacle cylinders, and
 * the avoidance envelope. */

#include "structure_bsp.h"
#include "collision_bsp.h"
#include "real_point3d.h"
#include "real_vector3d.h"
#include "vehicle_avoidance_cylinder.h"

typedef struct vector_avoidance_data
{
    const structure_bsp        *structure;                  /* 0x00 */
    const collision_bsp        *bsp;                        /* 0x04 */
    int                         object_index;               /* 0x08 */
    real_point3d                origin;                     /* 0x0C */
    real_vector3d               forward;                    /* 0x18 */
    real_vector3d               left;                       /* 0x24 */
    real_vector3d               up;                         /* 0x30 */
    __int16                     avoidance_object_count;     /* 0x3C (2 bytes pad) */
    unsigned char _pad0[2]; /* db-verified padding */
    vehicle_avoidance_cylinder  avoidance_objects[1024];    /* 0x40 */
    float                       avoid_width;                /* 0x6040 */
    float                       avoid_distance;             /* 0x6044 */
} vector_avoidance_data;                                    /* 24648 bytes */
