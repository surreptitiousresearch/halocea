/* physical_memory_free @0x836E05A8 — release the physical memory regions reserved at boot. */

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
