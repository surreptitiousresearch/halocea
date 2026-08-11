/* play_multiplayer_hit_sound @0x836B1618 — play the multiplayer "you hit something" feedback sound,
 * rate-limited to once every 2 game ticks. */

#include <stdint.h>
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);

void play_multiplayer_hit_sound(void)
{
    if ( should_play_multiplayer_hit_sound == 1 && game_time_get() > g_last_multiplayer_hit_sound_time + 2 )
    {
        game_engine_play_multiplayer_sound(_multiplayer_sound_ting, 0);
        g_last_multiplayer_hit_sound_time = game_time_get();
    }
}
