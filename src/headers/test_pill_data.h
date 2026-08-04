/* test_pill_data — working state threaded through the recursive collision-BSP pill sweep
 * (collision_bsp_test_pill → bsp3d_test_pill_recursive → bsp2d_test_pill_recursive). Layout taken
 * verbatim from the database type of the same name. */
#pragma once

#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_point2d.h"
#include "real_vector2d.h"
#include "collision_bsp.h"
#include "collision_bsp_test_pill_result.h"

typedef struct test_pill_data
{
    const collision_bsp            *bsp;             /* 0x000 */
    const real_point3d             *point;           /* 0x004 */
    const real_vector3d            *vector;          /* 0x008 */
    float                           radius;          /* 0x00C */
    collision_bsp_test_pill_result *result;          /* 0x010 */
    int                             stack_depth;     /* 0x014 */
    int                             plane_stack[128]; /* 0x018 */
    __int16                         projection_axis; /* 0x218 */
    unsigned char                   projection_sign; /* 0x21A */
    unsigned char _pad0[1]; /* db-verified padding */
    real_point2d                    p2d;             /* 0x21C */
    real_vector2d                   v2d;             /* 0x224 */
} test_pill_data;
