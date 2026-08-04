/* rasterizer_decal_vertices_purge_proc @0x83710CF8 — tail-call forwarder (`b decal_delete`; confirmed
 * via disasm, not a bare blr). This is a generic block-array purge callback whose "block index" is, for
 * the decal vertex block array, exactly the decal index — so the retail build wired it straight to
 * decal_delete. decal_delete itself is not yet decompiled — declared extern with its own DB prototype
 * and called through. */

#include <stdint.h>

extern void decal_delete(uint16_t decal_index);

void rasterizer_decal_vertices_purge_proc(int block_index)
{
    decal_delete(block_index);
}
