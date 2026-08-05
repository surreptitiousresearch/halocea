#pragma once
/* rendered_cluster — one visible structure cluster recorded for the current render pass, with the
 * sub-frustum it was reached through. Layout from the database (416 bytes). */

#include <stdint.h>
#include "real_rectangle2d.h"
#include "render_frustum.h"

typedef struct rendered_cluster
{
    int16_t          cluster_index; /* 0x00 */
    unsigned char _pad0[2]; /* db-verified padding */
    real_rectangle2d clip_bounds;   /* 0x04 */
    render_frustum   frustum;       /* 0x14 */
} rendered_cluster;                 /* 0x1A0 */
