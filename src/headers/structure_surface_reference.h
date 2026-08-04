#pragma once
/* structure_surface_reference — one entry in a structure BSP's surface_references tag_block (8 bytes).
 * DB-verified layout: the referenced surface and the bsp3d node that owns it. */

typedef struct structure_surface_reference
{
    int surface_index;      /* 0x00 */
    int bsp3d_node_index;   /* 0x04 */
} structure_surface_reference;  /* 8 bytes */
