#pragma once
/* structure_leaf — a leaf of the structure BSP's leaf array (DB-verified, 16 bytes). */

#include "byte_rectangle3d.h"

typedef struct structure_leaf
{
    byte_rectangle3d bounds;            /* 0x00 */
    unsigned __int16 pad;               /* 0x06 */
    __int16 cluster_index;              /* 0x08 */
    __int16 surface_reference_count;    /* 0x0A */
    int first_surface_reference_index;  /* 0x0C */
} structure_leaf;
