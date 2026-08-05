/* test_sphere_data — working state threaded through the recursive collision-BSP sphere overlap query
 * (collision_bsp_test_sphere → bsp3d_test_sphere_recursive). Layout verbatim from the database. */
#pragma once

#include <stdint.h>
#include "real_point3d.h"
#include "real_point2d.h"
#include "collision_bsp.h"
#include "collision_bsp_test_sphere_result.h"

typedef struct test_sphere_data
{
    const collision_bsp              *bsp;                     /* 0x000 */
    int16_t                           breakable_surface_count; /* 0x004 */
    unsigned char _pad0[2]; /* db-verified padding */
    const unsigned char              *breakable_surface_flags; /* 0x008 */
    const real_point3d               *center;                  /* 0x00C */
    float                             radius;                  /* 0x010 */
    collision_bsp_test_sphere_result *result;                  /* 0x014 */
    int                               stack_depth;             /* 0x018 */
    int                               plane_stack[128];        /* 0x01C */
    int16_t                           projection_axis;         /* 0x21C */
    unsigned char                     projection_sign;         /* 0x21E */
    unsigned char _pad1[1]; /* db-verified padding */
    real_point2d                      center2d;                /* 0x220 */
} test_sphere_data;
