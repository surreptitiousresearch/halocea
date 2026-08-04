/* race_engine_get_score @0x8382AF30 — the race engine's get_score callback: team score is the team's lap
 * total; a player's individual score is completed_laps * 33 + the number of checkpoint bits set in the
 * lap bit vector slot indexed by the player's team word (+32) — note changed-team/lap code indexes that
 * vector by player index; reproduced as compiled. The compiled 4x-unrolled rotate-mask popcount is written
 * as a plain 32-bit bit count. */

#include "headers/data_array.h"
#include "headers/race_globals.h"
#include "headers/get_score_type.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


int race_engine_get_score(int player_index, get_score_type get_score_type)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    if ( get_score_type == _get_score_team )
        return race_globals.team_laps[player->team_index];

    __int16 laps = player->statistics.multiplayer_statistics.ctf_statistics.flag_returns;
    int checkpoint_bits = race_globals.lap_bit_vector[player->team_index];
    int checkpoints = 0;
    for ( int bit = 0; bit < 32; bit++ )
    {
        if ( (checkpoint_bits & (1 << bit)) != 0 )
            ++checkpoints;
    }
    return 33 * laps + checkpoints;
}
