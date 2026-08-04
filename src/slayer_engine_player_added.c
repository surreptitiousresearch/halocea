/* slayer_engine_player_added @0x838153D8 — mark the player's multiplayer.special field as "no timestamp"
 * (-1), and (as server) reset their slayer score (and team score bucket, in a non-team game). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/slayer_globals.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

extern int16_t game_connection(void);
extern uint8_t game_engine_has_teams(void);

void slayer_engine_player_added(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    player->multiplayer.special = -1;
    if ( game_connection() == _game_connection_network_server )
    {
        slayer_globals.individual_score[(unsigned __int16)player_index] = 0;
        if ( !game_engine_has_teams() )
            slayer_globals.team_score[player->team_index] = 0;
    }
}
