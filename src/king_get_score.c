#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/king_globals.h"
#include "headers/get_score_type.h"
#include "headers/blam_data_globals.h"


int king_get_score(int player_index, get_score_type get_score_type)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    if (get_score_type)
        return king_globals.score[player->team_index];

    /* individual score = statistics.multiplayer_statistics.king_statistics.time_on_hill (player_datum+0xC4, game_statistics+0x34) */
    return player->statistics.multiplayer_statistics.king_statistics.time_on_hill;
}
