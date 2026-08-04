#pragma once

/* bsp2d_node — one interior node of a 2D BSP split tree (20 bytes). A dividing
 * plane and the two child links; a negative child index encodes a leaf/surface
 * reference (top bit set). DB type bsp2d_node. */

#include "real_plane2d.h"

typedef struct bsp2d_node
{
    real_plane2d plane;         /* 0x00 */
    int          child_indices[2]; /* 0x0C — [0] back, [1] front */
} bsp2d_node;                   /* 0x14 */
