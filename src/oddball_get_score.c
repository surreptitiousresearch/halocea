#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/oddball_globals.h"
#include "headers/get_score_type.h"
#include "headers/blam_data_globals.h"


int oddball_get_score(int player_index, get_score_type get_score_type)
{
    if (get_score_type == _get_score_team)
    {
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
        return oddball_globals.team_score[player->team_index];
    }

    return oddball_globals.individual_score[player_index];
}
