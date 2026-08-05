/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* race_team_can_win_game @0x8382A610 — a race team "can win" (defaults true) unless single-flag-race mode is
 * active AND some player on that team is both under the score-to-win threshold and out of lives — any such
 * player eliminates the team's chance to win. Player datum's team_index (+32) and score (+198) accessed by
 * raw offset (see race_engine_did_player_win.c for the same team_index offset). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/game_variant.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


extern game_variant *game_engine_get_variant(void);
extern uint8_t game_engine_player_is_out_of_lives(int player_index);

uint8_t race_team_can_win_game(int team_index)
{
    uint8_t can_win = 1;

    if ( game_engine_get_variant()->game_engine_variant.ctf.single_flag_time )
        return can_win;

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);

    for ( player_datum *player = data_iterator_next(&iterator); player;
          player = data_iterator_next(&iterator) )
    {
        if ( player->team_index == team_index
          && player->statistics.multiplayer_statistics.ctf_statistics.flag_returns
                 < game_engine_get_variant()->universal_variant.score_to_win )
        {
            if ( game_engine_player_is_out_of_lives(iterator.index) )
                can_win = 0;
        }
    }

    return can_win;
}
