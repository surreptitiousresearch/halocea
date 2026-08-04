#pragma once
/* render_mirror — a reflective surface found during visibility, used to render a mirrored reflection pass.
 * Layout from the database (28 bytes). */

#include "real_plane3d.h"

typedef struct render_mirror
{
    real_plane3d plane;               /* 0x00 — mirror plane */
    float        index_of_refraction; /* 0x10 */
    float        depth;               /* 0x14 */
    __int16      cluster_index;       /* 0x18 — cluster the reflection renders from */
    unsigned char _pad1A[2];          /* 0x1A — tail padding to 4-byte alignment */
} render_mirror;                      /* 28 bytes */
