/* collision_bsp_test_vector_result — result block filled by collision_bsp_test_vector: the nearest
 * surface hit along the ray plus the list of BSP leaves the ray passed through (1048 bytes). */
#pragma once

#include "real_plane3d.h"

typedef struct collision_bsp_test_vector_result
{
    float               t;                        /* 0x00 */
    const real_plane3d *plane;                    /* 0x04 */
    int                 surface_index;            /* 0x08 */
    int                 plane_designator;         /* 0x0C */
    unsigned char       flags;                    /* 0x10 */
    unsigned char       breakable_surface_index;  /* 0x11 */
    __int16             material_index;           /* 0x12 */
    int                 leaf_count;               /* 0x14 */
    int                 leaf_indices[256];        /* 0x18 */
} collision_bsp_test_vector_result;
