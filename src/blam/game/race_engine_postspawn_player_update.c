/* race_engine_postspawn_player_update @0x8382AE50 — on a networked client (game_connection() == _game_connection_network_server), spawn a
 * race vehicle for a player who hasn't died yet (deaths field at +174, see
 * game_engine_replicate_score_and_state_to_network.c) as long as fewer vehicles have spawned than there are
 * player slots. Always returns 1. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/race_globals.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"


extern int16_t game_connection(void);
extern void create_race_vehicle(int player_index);

uint8_t race_engine_postspawn_player_update(int player_index)
{
    if ( game_connection() == _game_connection_network_server
        && !DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->statistics.deaths
        && race_globals.number_of_spawned_vehicles < player_data->actual_count )
    {
        create_race_vehicle(player_index);
    }

    return 1;
}
