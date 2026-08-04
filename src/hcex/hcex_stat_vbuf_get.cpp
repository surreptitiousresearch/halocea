/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
#include <stdint.h>
#include "../headers/d3d_boundary.h"

/*
 * Accessor for the D3D vertex buffer owned by a static-geometry buffer set.
 * The set stores a pointer to its GPU resource block at word offset 2 (byte 8);
 * the vertex buffer handle sits at byte 0x64 within that block. No DB struct
 * covers either layer, so raw offsets are retained.
 */
extern "C" D3DVertexBuffer *hcex_stat_vbuf_get(uint32_t *static_buffer_set)
{
    uintptr_t resource_block = static_buffer_set[2];
    return *(D3DVertexBuffer **)(resource_block + 0x64);
}
