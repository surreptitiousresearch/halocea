/* physical_memory_free @0x836E05A8 — release the physical memory regions reserved at boot.
 *
 * CAVEAT (documentation, not a defect): dead in the shipped binary. The function has zero code
 * xrefs — its only reference is the .pdata unwind entry at 0x8236A490 — so the boot reservation is
 * never released. Its counterpart physical_memory_allocate @0x836E0508 is likewise reached only
 * from the C++ static constructor ??0HCEX_INIT_STAT@@QAA@XZ (bl @0x823E3B90), before Blam starts. */

#include "headers/physical_memory_map_globals.h"

/* Xbox boundary. */
extern void XPhysicalFree(void *address);

void physical_memory_free(void)
{
    if ( physical_memory_map_globals.game_state_base_address )
        XPhysicalFree(physical_memory_map_globals.game_state_base_address);
    if ( physical_memory_map_globals.tag_cache_base_address )
        XPhysicalFree(physical_memory_map_globals.tag_cache_base_address);
}
