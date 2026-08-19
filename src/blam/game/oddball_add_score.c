/* oddball_add_score @0x838161B0 — award one score tick to a player's individual and team totals
 * (only while the connection is a server, per game_connection() == _game_connection_network_server), play a milestone sound at 900 and
 * 1800 points remaining until score_to_win (using the teams-vs-free-for-all sound bank), then end the
 * game once the scoring team reaches score_to_win. */

#include <stdint.h>
#include "headers/oddball_globals.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/game_connection.h"
#include "headers/multiplayer_team.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/blam_data_globals.h"


extern int16_t game_connection(void);
extern uint8_t game_engine_has_teams(void);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern void game_engine_end_game(void);

void oddball_add_score(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int team_index = player->team_index;

    if ( game_connection() == _game_connection_network_server )
    {
        ++oddball_globals.individual_score[(uint16_t)player_index];
        ++oddball_globals.team_score[team_index];

        if ( oddball_globals.score_to_win - oddball_globals.team_score[team_index] == 900 )   /* 30 s remaining @ 30 ticks/s */
        {
            int sound = game_engine_has_teams()
                    ? (team_index == _multiplayer_team_red ? _multiplayer_sound_red_30_seconds : _multiplayer_sound_blue_30_seconds)
                    : _multiplayer_sound_30_seconds;
            game_engine_play_multiplayer_sound(sound, 1u);
        }

        if ( oddball_globals.score_to_win - oddball_globals.team_score[team_index] == 1800 )   /* 60 s remaining */
        {
            int sound = game_engine_has_teams()
                    ? (team_index == _multiplayer_team_red ? _multiplayer_sound_red_60_seconds : _multiplayer_sound_blue_60_seconds)
                    : _multiplayer_sound_60_seconds;
            game_engine_play_multiplayer_sound(sound, 1u);
        }
    }

    if ( oddball_globals.team_score[team_index] >= oddball_globals.score_to_win )
        game_engine_end_game();
}
