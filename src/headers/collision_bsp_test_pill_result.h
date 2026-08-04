/* collision_bsp_test_pill_result — output of collision_bsp_test_pill: the first contact fraction (t), the
 * surface plane/material hit, and the list of structure-BSP leaves the swept pill passed through. Layout taken
 * verbatim from the database type of the same name. */
#pragma once

#include "real_plane3d.h"

typedef struct collision_bsp_test_pill_result
{
    float           t;                       /* 0x00 — contact fraction along the sweep */
    real_plane3d    plane;                   /* 0x04 — surface plane hit */
    int             surface_index;           /* 0x14 */
    unsigned char   flags;                   /* 0x18 */
    unsigned char   breakable_surface_index; /* 0x19 */
    __int16         material_index;          /* 0x1A */
    int             leaf_count;              /* 0x1C */
    int             leaf_indices[256];       /* 0x20 — leaf_count entries */
} collision_bsp_test_pill_result;
