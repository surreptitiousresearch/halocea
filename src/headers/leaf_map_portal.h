#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* leaf_map_portal — one portal record of a leaf_map (leaf_map->portals, 24-byte stride, DB layout confirmed via
 * disasm at 0x8381B9AC: three full-word stores at offsets 0/4/8, then a tag_block at 0xC). A portal bridges two
 * leaves across a BSP splitting plane; its polygon vertices are the projected intersection of the two leaves'
 * faces on that plane. The decompiler mis-types this as a bare tag_block (rendering plane_index as `->count`);
 * it is really three ints plus the vertex tag_block. */

#include "tag_block.h"

typedef struct leaf_map_portal
{
    int       plane_index;   /* 0x00 — the BSP node's splitting-plane index */
    int       leaf_index0;   /* 0x04 — masked (& 0x7FFFFFFF) first leaf index */
    int       leaf_index1;   /* 0x08 — masked second leaf index */
    tag_block vertices;      /* 0x0C — real_point3d each (projected portal polygon) */
} leaf_map_portal;           /* 0x18 = 24 bytes */
