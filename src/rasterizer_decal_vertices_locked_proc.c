/* rasterizer_decal_vertices_locked_proc @0x83710D00 — LRU-vertex-cache callback: a decal's vertices are
 * considered locked (must not be evicted/reused) if it is permanent (flags bit 2) or NOT locked-for-update
 * (flags bit 1 clear) — i.e. only a decal that's both temporary and currently locked-for-update is free
 * to be evicted. Records the queried decal as the cache's last-queried index either way. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/decal_datum.h"
#include "headers/decal_flags.h"
#include "headers/blam_data_globals.h"


/* return respelled int->uint8_t 2026-07-30: disasm clrlwi r3,r10,24 byte-normalizes at blr, and the
 * lruv locked_block_proc slot is uint8_t(*)(int) corpus-wide (C4133 at _rasterizer_decals_initialize) */
uint8_t rasterizer_decal_vertices_locked_proc(int block_index)
{
    decal_datum *decal = (decal_datum *)global_decal_data->data + (unsigned __int16)block_index;
    unsigned __int16 flags = decal->flags;

    int locked = (flags & (1u << _decal_permanent_bit)) != 0 ||
                 (flags & (1u << _decal_locked_bit)) == 0;

    last_decal_index_queried_by_lruv_cache = block_index;
    return locked;
}
