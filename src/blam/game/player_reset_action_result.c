/* player_reset_action_result @0x836A97E8 — clear a player's pending action: reset the action
 * result code and invalidate the action object index. */

#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


void player_reset_action_result(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    player->action_result = 0;
    player->action_object_index = -1;
}
