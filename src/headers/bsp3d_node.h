#pragma once
/* bsp3d_node — one split node of a bsp3d tree. Child fields are node indices when non-negative;
 * negative values encode a leaf (top bit set) or -1 for "no geometry on this side". */

typedef struct bsp3d_node
{
    int plane_index;       /* 0x00 */
    int child_indices[2];  /* 0x04 - [0]=back child, [1]=front child */
} bsp3d_node;              /* 12 bytes */
