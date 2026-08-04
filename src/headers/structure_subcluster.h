#pragma once
/* structure_subcluster — one subcluster of a structure_cluster's subcluster tree (structures.c).
 * 36 bytes, DB types_members-confirmed layout. */

#include "tag_block.h"
#include "real_rectangle3d.h"

typedef struct structure_subcluster
{
    real_rectangle3d world_bounds;      /* 0x00 */
    tag_block        surface_indices;   /* 0x18 — surface_index count@0x18, address@0x1C */
} structure_subcluster;                 /* 36 bytes */
