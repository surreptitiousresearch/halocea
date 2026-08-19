/* ctf_sound_update_warning @0x83806B98 — on the network server (game_connection() == _game_connection_network_server; value 2 = server per headers_ref enum), while a team's
 * flag-carrier warning is active, play the periodic warning sound once its timer exceeds 600 ticks
 * (resetting the timer), then advance the timer every call. */

#include <stdint.h>
#include "headers/ctf_globals.h"
#include "headers/game_connection.h"
#include "headers/multiplayer_team.h"
#include "headers/game_engine_multiplayer_sound.h"

extern int16_t game_connection(void);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);

void ctf_sound_update_warning(int team_index)
{
    if ( game_connection() == _game_connection_network_server && ctf_globals.flag_warning[team_index] )
    {
        if ( ctf_globals.flag_warning_time_in_ticks[team_index] > 600 )
        {
            game_engine_play_multiplayer_sound(team_index == _multiplayer_team_red
                    ? _multiplayer_sound_ctf_blue_took_flag
                    : _multiplayer_sound_ctf_red_took_flag, 1u);
            ctf_globals.flag_warning_time_in_ticks[team_index] = 0;
        }

        ++ctf_globals.flag_warning_time_in_ticks[team_index];
    }
}
