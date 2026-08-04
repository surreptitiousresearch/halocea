#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/ctf_globals.h"
#include "headers/get_score_type.h"
#include "headers/blam_data_globals.h"


int ctf_get_score(int player_index, get_score_type get_score_type)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    if (get_score_type)
        return ctf_globals.score[player->team_index];

    /* individual score = statistics.multiplayer_statistics.ctf_statistics.flag_scores (player_datum+0xC8, game_statistics+0x38) */
    return player->statistics.multiplayer_statistics.ctf_statistics.flag_scores;
}
