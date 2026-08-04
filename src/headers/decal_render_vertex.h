#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* decal_render_vertex — the 16-byte per-vertex record decal_new_from_collision builds for the
 * rasterizer decal vertex buffer. Layout derived from the observed store pattern (position at 0x00,
 * a packed 8:8 texel pair at 0x0C); this is a runtime render vertex, not a tag type. */

#include "real_point3d.h"

typedef struct decal_render_vertex
{
    real_point3d position;        /* 0x00 */
    unsigned int packed_texcoord; /* 0x0C — (u & 0xFF) << 16 | (v & 0xFF) << 8 */
} decal_render_vertex;            /* 16 bytes */
