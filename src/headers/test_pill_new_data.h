#pragma once
/* test_pill_new_data — recursion state for collision_bsp_test_pill_new_recursive's BSP-tree pill sweep.
 * DB layout, 44 bytes. */

#include "collision_bsp.h"
#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct test_pill_new_data
{
    const collision_bsp     *bsp;                      /* 0x00 */
    __int16                  breakable_surface_count;   /* 0x04 */
    unsigned __int8          pad_06[2];                  /* 0x06 */
    const unsigned __int8   *breakable_surface_flags;   /* 0x08 */
    const real_point3d      *point;                     /* 0x0C */
    const real_vector3d     *vector;                    /* 0x10 */
    float                    radius;                    /* 0x14 */
    float                   *t;                          /* 0x18 */
    real_vector3d           *normal;                    /* 0x1C */
    int                      last_leaf_index;            /* 0x20 */
    unsigned __int8          last_contents;              /* 0x24 */
    unsigned __int8          pad_25[3];                  /* 0x25 */
    int                      last_plane_designator;      /* 0x28 */
} test_pill_new_data; /* 44 bytes */
