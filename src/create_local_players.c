/* create_local_players @0x83689044 — create the local player records at game start. In the main menu
 * one player is created in slot 0; in-game, one is created per requested spawn into a free local-player
 * slot. */

#include "headers/main_globals.h"
#include "headers/blam_data_globals.h"

#include <stdint.h>

typedef struct network_player network_player;
extern int player_new(int machine_index, int player_index, uint16_t local_player_index, network_player *network_player_data);
extern void local_player_set_player_index(int16_t local_player_index, int player_index);
extern int16_t find_unused_local_player_index(void);

void create_local_players(void)
{
    if ( main_globals.main_menu_scenario_loaded )
    {
        int player_index = player_new(0, -1, 0, 0);
        local_player_set_player_index(0, player_index);
    }
    else
    {
        for ( int i = 0; i < player_spawn_count; ++i )
        {
            /* (int16_t) models extsh r29,r3 @0x8368908C — result narrowed before reuse */
            int16_t local_player_index = find_unused_local_player_index();
            int player_index = player_new(0, -1, local_player_index, 0);
            local_player_set_player_index(local_player_index, player_index);
        }
    }
}
