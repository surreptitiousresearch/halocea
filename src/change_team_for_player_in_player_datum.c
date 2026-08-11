/* change_team_for_player_in_player_datum @0x83802A60 — finds the player_datum whose embedded
 * network_player_data.player_list_index matches `player_index` and updates both its
 * network_player_data.team_index and its own top-level team_index field to `team_index`. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


void change_team_for_player_in_player_datum(int player_index, uint8_t team_index)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);

    player_datum *player = data_iterator_next(&iterator);
    if ( !player )
        return;

    while ( player->network_player_data.player_list_index != player_index )
    {
        player = data_iterator_next(&iterator);
        if ( !player )
            return;
    }

    player->network_player_data.team_index = team_index;
    player->team_index = team_index;
}
