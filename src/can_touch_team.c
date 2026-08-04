/* can_touch_team @0x8382A4D0 — race engine: whether `player_index` may currently touch team
 * `team_index`'s flag. Denied outright for an invalid team (>=32) or once the player has already reached
 * the score-to-win lap count (+198, a reused multiplayer_statistics union scratch field repurposed for race
 * lap tracking, statistics.multiplayer_statistics.race_statistics.laps). In "flag rally" races, only
 * the current rally-flag team may be touched. Otherwise:
 * once the player has completed every lap-flag for their current lap (lap_bit_vector == lap_completed_value,
 * i.e. lap just finished/about to restart), only their recorded first-touched team may be touched again;
 * mid-lap, only a team whose flag hasn't been touched yet this lap qualifies, and for a normal-order race
 * it must additionally be the next untouched team at or after `team_index` in ascending order (skipping
 * already-touched teams) — any already-touched team encountered strictly before reaching `team_index`
 * disqualifies it. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/race_globals.h"
#include "headers/game_variant.h"
#include "headers/blam_data_globals.h"


extern game_variant *game_engine_get_variant(void);

uint8_t can_touch_team(int player_index, int team_index)
{
    if ( team_index >= 32 )
        return 0;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    if ( player->statistics.multiplayer_statistics.race_statistics.laps
            >= game_engine_get_variant()->universal_variant.score_to_win )
        return 0;

    if ( game_engine_get_variant()->game_engine_variant.terminator.ignored == 2 )
        return team_index == race_globals.rally_flag;

    int untouched_teams_this_lap = race_globals.lap_completed_value & ~race_globals.lap_bit_vector[player_index];

    if ( race_globals.lap_bit_vector[player_index] == race_globals.lap_completed_value )
        return race_globals.first_flag[player_index] == team_index;

    if ( ((1 << team_index) & untouched_teams_this_lap) == 0 )
        return 0;

    race_type race_type = game_engine_get_variant()->game_engine_variant.race.race_type;
    if ( race_type == _race_type_normal )
    {
        for ( int candidate_team = race_type; candidate_team != team_index; candidate_team++ )
        {
            if ( (1 << candidate_team) & untouched_teams_this_lap )
                return 0;
            if ( candidate_team + 1 >= 32 )
                return 1;
        }
    }

    return 1;
}
