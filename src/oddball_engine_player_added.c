/* oddball_engine_player_added @0x83816040 — reset a newly-added player's oddball score (and, in a
 * non-team game, their team's score bucket) when acting as the network server. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/oddball_globals.h"
#include "headers/player_datum.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

extern int16_t game_connection(void);
extern uint8_t game_engine_has_teams(void);

void oddball_engine_player_added(int player_index)
{
    if ( game_connection() == _game_connection_network_server )
    {
        oddball_globals.individual_score[player_index] = 0;
        if ( !game_engine_has_teams() )
        {
            player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

            oddball_globals.team_score[player->team_index] = 0;
        }
    }
}
