/* slayer_get_score @0x83815740 */
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/slayer_globals.h"
#include "headers/get_score_type.h"
#include "headers/blam_data_globals.h"


int slayer_get_score(int player_index, get_score_type get_score_type)
{
    if (get_score_type == _get_score_team)
    {
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
        return slayer_globals.team_score[player->team_index];
    }

    return slayer_globals.individual_score[player_index];
}
