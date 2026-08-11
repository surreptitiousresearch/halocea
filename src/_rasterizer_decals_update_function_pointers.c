/* _rasterizer_decals_update_function_pointers @0x83710DE0 — re-point the decal vertex LRUV cache at
 * its two callbacks (tail-calls lruv_update_function_pointers).
 *
 * DEVIATION: both callbacks were declared here as funcptr OBJECTS
 * (`extern void (*rasterizer_decal_vertices_purge_proc)(int);`), while `src/<name>.c` defines each
 * as a FUNCTION and the sibling `_rasterizer_decals_initialize.c` declares them as such. Both
 * spellings link to the same symbol, so nothing complained — but the object form makes the
 * compiler LOAD from the address, i.e. pass the callee's first instruction word as the callback.
 * The binary settles it: `lis r10/r9 @0x83710DE4-E8` + `addi r5/r4 @0x83710DEC-F0` take the
 * ADDRESSES, while `local_vertex_cache` one instruction later is `lis r11` + `lwz r3
 * @0x83710DF4` — a genuine variable load. Same-file contrast, opposite instruction. */

#include <stdint.h>

#include "headers/lruv_cache.h"
#include "headers/blam_data_globals.h"

extern void rasterizer_decal_vertices_purge_proc(int block_index);
extern uint8_t rasterizer_decal_vertices_locked_proc(int block_index);
extern void lruv_update_function_pointers(lruv_cache *cache,
                                           void (*delete_block_proc)(int),
                                           unsigned char (*locked_block_proc)(int));

void _rasterizer_decals_update_function_pointers(void)
{
    lruv_update_function_pointers(
        local_vertex_cache,
        rasterizer_decal_vertices_purge_proc,
        rasterizer_decal_vertices_locked_proc);
}
