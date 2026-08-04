/* local_player_set_player_index @0x836A805C — bind a local player slot to a player datum index:
 * clear the back-reference on the previously-bound player, store the new player index in the slot,
 * and write this local-player index into the new player datum's local-player field (player datum +2). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/players_global_data.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


void local_player_set_player_index(int16_t local_player_index, int player_index)
{
    players_global_data *globals = players_globals;
    int previous_player_index = globals->local_player_indices[local_player_index];
    if ( previous_player_index != -1 )
    {
        DATA_ARRAY_ELEMENT(player_data, player_datum, previous_player_index)->local_player_index = -1;
        globals = players_globals;
    }
    globals->local_player_indices[local_player_index] = player_index;
    if ( player_index != -1 )
        DATA_ARRAY_ELEMENT(player_data, player_datum, players_globals->local_player_indices[local_player_index])->local_player_index = local_player_index;
}
