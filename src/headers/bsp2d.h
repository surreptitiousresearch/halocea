/* bsp2d — a 2D binary space partition used to resolve which surface within a BSP leaf contains a
 * projected point. Nodes (20 bytes each): float line[3] (a,b,c of a 2D plane) + int back/front
 * children (leaf children are negative). */
#pragma once

#include "tag_block.h"

typedef struct bsp2d
{
    tag_block nodes; /* 0x00 */
} bsp2d;
