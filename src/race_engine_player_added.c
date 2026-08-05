/* race_engine_player_added @0x8382B4D8 — stamp the player's multiplayer.special field with the current
 * game time, clear their lap bit vector, and (as server) recompute team scores. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/race_globals.h"
#include "headers/game_connection.h"
#include "headers/game_variant.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);
extern int16_t game_connection(void);
extern void race_update_team_score(void);

void race_engine_player_added(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    player->multiplayer.special = game_time_get();
    race_globals.lap_bit_vector[player_index] = 0;
    if ( game_connection() == _game_connection_network_server )
        race_update_team_score();
}
