/* ctf_engine_update @0x83807990 — per-frame CTF engine tick: end the game once either team reaches
 * score_to_win, and for each team whose flag is currently on warning (server only), play the "flag
 * warning" sound (index 8 for team 0, 11 for team 1) once the warning timer exceeds 600 ticks and reset
 * it, otherwise advance the timer. */

#include <stdint.h>
#include "headers/ctf_globals.h"
#include "headers/game_connection.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/blam_data_globals.h"


extern void game_engine_end_game(void);
extern int16_t game_connection(void);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);

void ctf_engine_update(void)
{
    if ( ctf_globals.score[0] >= ctf_globals.score_to_win || ctf_globals.score[1] >= ctf_globals.score_to_win )
        game_engine_end_game();

    if ( game_connection() == _game_connection_network_server && ctf_globals.flag_warning[0] )
    {
        int ticks = ctf_globals.flag_warning_time_in_ticks[0];
        if ( ticks > 600 )
        {
            game_engine_play_multiplayer_sound(_multiplayer_sound_ctf_blue_took_flag, 1u);
            ticks = 0;
        }
        ctf_globals.flag_warning_time_in_ticks[0] = ticks + 1;
    }

    if ( game_connection() == _game_connection_network_server && ctf_globals.flag_warning[1] )
    {
        int ticks = ctf_globals.flag_warning_time_in_ticks[1];
        if ( ticks > 600 )
        {
            game_engine_play_multiplayer_sound(_multiplayer_sound_ctf_red_took_flag, 1u);
            ticks = 0;
        }
        ctf_globals.flag_warning_time_in_ticks[1] = ticks + 1;
    }
}
