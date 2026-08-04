/* race_touch_flag @0x8382B6B0 — race engine: a player touches a placed netgame flag. If the player's team
 * may touch this flag's team, play the touch sound, claim it as the team's "first flag" if none is set
 * yet, and either complete the lap immediately (terminator/"rally race" variant) or toggle the flag's bit
 * in the team's per-flag lap bit vector — completing the lap once every flag bit is set. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_netgame_flag.h"
#include "headers/race_globals.h"
#include "headers/game_variant.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/blam_data_globals.h"


extern uint8_t can_touch_team(int player_index, int team_index);
extern void game_engine_play_multiplayer_sound_to_specific_player(int player_index, int index, uint8_t should_replicate);
extern game_variant *game_engine_get_variant(void);
extern void race_complete_lap(int player_index);
extern int new_rally_flag(int old_flag);

void race_touch_flag(int player_index, int netgame_flag_index)
{
    scenario_netgame_flag *flag = &((scenario_netgame_flag *)global_scenario->netgame_flags.address)[netgame_flag_index];
    int team_index = flag->team_index;

    if ( can_touch_team(player_index, team_index) )
    {
        game_engine_play_multiplayer_sound_to_specific_player(player_index,
            _multiplayer_sound_countdown_timer, 1u);

        int *first_flag = &race_globals.first_flag[player_index];
        if ( *first_flag == -1 )
            *first_flag = team_index;   /* per-player: the team of the first flag this player touched */

        if ( game_engine_get_variant()->game_engine_variant.terminator.ignored == 2 )
        {
            race_complete_lap(player_index);
            race_globals.rally_flag = new_rally_flag(race_globals.rally_flag);
        }
        else
        {
            int lap_bits = race_globals.lap_bit_vector[player_index];
            if ( lap_bits == race_globals.lap_completed_value )
                race_complete_lap(player_index);
            else
                race_globals.lap_bit_vector[player_index] = (1 << team_index) | lap_bits;
        }
    }
}
