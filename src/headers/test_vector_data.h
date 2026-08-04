/* test_vector_data — the working state threaded through the recursive collision-BSP vector test:
 * the ray and filter parameters, the result block being filled, and the running record of the last
 * leaf/contents/plane crossed (40 bytes). */
#pragma once

#include "real_point3d.h"
#include "real_vector3d.h"
#include "collision_bsp_test_vector_result.h"

struct collision_bsp;

typedef struct test_vector_data
{
    unsigned int                       flags;                   /* 0x00 */
    const struct collision_bsp        *bsp;                     /* 0x04 */
    __int16                            breakable_surface_count; /* 0x08 */
    unsigned char _pad0[2]; /* db-verified padding */
    const unsigned char               *breakable_surface_flags; /* 0x0C */
    const real_point3d                *point;                   /* 0x10 */
    const real_vector3d               *vector;                  /* 0x14 */
    collision_bsp_test_vector_result  *result;                  /* 0x18 */
    int                                last_leaf_index;         /* 0x1C */
    unsigned char                      last_contents;           /* 0x20 */
    unsigned char _pad1[3]; /* db-verified padding */
    int                                last_plane_index;        /* 0x24 */
} test_vector_data;
