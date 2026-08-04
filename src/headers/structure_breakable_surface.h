#pragma once
/* structure_breakable_surface — one element of structure_bsp.breakable_surfaces (48 bytes; from
   headers_ref, matching this binary's actual usage). NOTE: the DB type of this name carries a
   different (tag-definition) layout; the runtime access here is centroid/radius/collision index. */

#include "real_point3d.h"

typedef struct structure_breakable_surface
{
    real_point3d centroid;                /* 0x00 */
    float        bounding_radius;         /* 0x0C */
    int          collision_surface_index; /* 0x10 */
    int          unused[7];               /* 0x14 */
} structure_breakable_surface;            /* 48 bytes */
