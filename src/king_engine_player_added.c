/* king_engine_player_added @0x8382BF2C — on a networked client (game_connection() == _game_connection_network_server) in a teamless King
 * of the Hill game, resets the newly-added player's own per-team score/score-tick slot (indexed by the
 * player's team_index, which for a teamless game engine doubles as a per-player slot). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/king_globals.h"
#include "headers/player_datum.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"


extern int16_t game_connection(void);
extern uint8_t game_engine_has_teams(void);

void king_engine_player_added(int player_index)
{
    if ( game_connection() == _game_connection_network_server && !game_engine_has_teams() )
    {
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
        int team_index = player->team_index;

        king_globals.score[team_index] = 0;
        king_globals.score_tick[team_index] = 0;
    }
}
