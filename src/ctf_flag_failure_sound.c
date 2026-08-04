#include <stdint.h>
#include "headers/ctf_globals.h"
#include "headers/game_engine_multiplayer_sound.h"

extern int game_time_get(void);
extern void game_engine_play_multiplayer_sound_to_specific_player(int player_index, int index, uint8_t should_replicate);

void ctf_flag_failure_sound(int player_index)
{
    if ( game_time_get() > ctf_globals.non_deterministic_next_flag_failure_time )
    {
        game_engine_play_multiplayer_sound_to_specific_player(player_index, _multiplayer_sound_flag_failure, 1u);
        ctf_globals.non_deterministic_next_flag_failure_time = game_time_get() + 120;
    }
}
