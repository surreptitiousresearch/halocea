#pragma once
/* map_leaf — one BSP leaf's face/portal-designator index lists (24 bytes, DB layout).
 * leaf_map.leaves is a tag_block of these. */

#include "tag_block.h"

typedef struct map_leaf
{
    tag_block faces;               /* 0x00 */
    tag_block portal_designators;  /* 0x0C — index array into leaf_map.portals */
} map_leaf; /* 24 bytes */
