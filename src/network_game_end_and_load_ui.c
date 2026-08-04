/* network_game_end_and_load_ui @0x83782A20 — tear down a finished network game's local state: if its
 * objects were loaded, return to the UI scenario, then clear the local-data block. */

#include <stdint.h>
#include "headers/network_game_data.h"

extern void main_load_ui_scenario(uint8_t precache_resources);

void network_game_end_and_load_ui(network_game_data *game)
{
    if ( game->local_data.game_objects_loaded )
        main_load_ui_scenario(1u);
    game->local_data.game_objects_loaded = 0;
    game->local_data.pad3[0] = 0;
    game->local_data.pad3[1] = 0;
    game->local_data.pad3[2] = 0;
}
