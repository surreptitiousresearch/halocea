/* slayer_engine_adjust_score @0x838155F8 — adds `adjustment` to a player's individual slayer score and to
 * their team's team score, skipped when game_connection() == _game_connection_network_client
 * (`cmpwi cr6,r11,1 / beq cr6,loc_83815668` @0x8381561C-20). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/slayer_globals.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

extern int16_t game_connection(void);

void slayer_engine_adjust_score(int player_index, int adjustment)
{
    if ( game_connection() != _game_connection_network_client )
    {
        int team_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->team_index;
        slayer_globals.team_score[team_index] += adjustment;
        slayer_globals.individual_score[player_index] += adjustment;
    }
}
