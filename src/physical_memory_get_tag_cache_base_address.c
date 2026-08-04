/* physical_memory_get_tag_cache_base_address @0x836E0608 — return the base address of the tag cache region.
 * DEVIATION: the decompiler put tag_cache_base_address@0; the DB-verified layout has it @4
 * (game_state_base_address@0). Reusing physical_memory_map_globals.h corrects the offset. */
#include "headers/physical_memory_map_globals.h"
void *physical_memory_get_tag_cache_base_address(void)
{
    return physical_memory_map_globals.tag_cache_base_address;
}
