#pragma once
/* leaf_portal — one portal between two BSP leaves (24 bytes, DB layout). leaf_map.portals is a
 * tag_block of these. */

#include "tag_block.h"

typedef struct leaf_portal
{
    int       plane_index;     /* 0x00 */
    int       leaf_indices[2]; /* 0x04 — the two leaves this portal connects */
    tag_block vertices;        /* 0x0C */
} leaf_portal; /* 24 bytes */
