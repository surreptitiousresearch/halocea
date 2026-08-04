/* physical_memory_get_game_state_base_address @ 0x836E05F8
   Accessor for the game-state physical memory region base. */

#include "headers/physical_memory_map_globals.h"

void *physical_memory_get_game_state_base_address(void)
{
    return physical_memory_map_globals.game_state_base_address;
}
