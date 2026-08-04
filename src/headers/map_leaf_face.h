#pragma once
/* map_leaf_face — one polygon face of a leaf_map leaf: the BSP node whose splitting plane it lies on, plus
 * its 2D projected vertex list. DB layout, 16 bytes. */

#include "tag_block.h"

typedef struct map_leaf_face
{
    int       node_index;  /* 0x0 */
    tag_block vertices;    /* 0x4 — real_point2d each */
} map_leaf_face;            /* 16 bytes */
