/* rasterizer_decal_vertices_purge_proc @0x83710CF8 — tail-call forwarder (`b decal_delete`; confirmed
 * via disasm, not a bare blr). This is a generic block-array purge callback whose "block index" is, for
 * the decal vertex block array, exactly the decal index — so the retail build wired it straight to
 * decal_delete.
 *
 * DEVIATION (2026-08-12, #134): the stale note here said decal_delete "is not yet decompiled".
 * src/decal_delete.c has existed since; the extern below is now stamped from its attested
 * signature, which this unit widened uint16_t -> int (see that file). */

#include <stdint.h>

extern void decal_delete(int decal_index);

void rasterizer_decal_vertices_purge_proc(int block_index)
{
    decal_delete(block_index);
}
