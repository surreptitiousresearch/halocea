/* race_engine_update @0x8382A7A8 — per-tick race engine update. On game tick 2 it plays the race-start
 * sound (race solo / team_race teams) and scans the scenario netgame flags for the first race-vehicle
 * (type 4) flag — a loop whose result the shipped code never uses (reproduced as compiled). Note type 4 is
 * _netgame_flag_race_vehicle, not race_track (which is type 3). In team games, a team that can
 * no longer win ends the game. Finally the per-player speed multipliers are rebuilt. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_netgame_flag.h"
#include "headers/netgame_flag_type.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern uint8_t game_engine_has_teams(void);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern uint8_t race_team_can_win_game(int team_index);
extern void game_engine_end_game(void);
extern void build_player_speeds(void);

void race_engine_update(void)
{
    if ( game_time_get() == 2 )
    {
        game_engine_play_multiplayer_sound(
            game_engine_has_teams() == 0 ? _multiplayer_sound_race : _multiplayer_sound_team_race, 0);

        /* dead scan for the first race-vehicle netgame flag — result unused, as compiled */
        scenario_netgame_flag *netgame_flag = (scenario_netgame_flag *)global_scenario->netgame_flags.address;
        for ( int i = 0; i < global_scenario->netgame_flags.count; i++, netgame_flag++ )
        {
            if ( netgame_flag->type == _netgame_flag_race_vehicle )
                break;
        }
    }

    if ( game_engine_has_teams() )
    {
        if ( !race_team_can_win_game(0) )
            game_engine_end_game();
        if ( !race_team_can_win_game(1) )
            game_engine_end_game();
    }
    build_player_speeds();
}
