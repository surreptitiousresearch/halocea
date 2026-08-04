#include <stdint.h>
#include "headers/game_engine_globals.h"
#include "headers/game_engine_multiplayer_sound.h"

extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern void ui_widgets_close_all(void);

void game_engine_progress_to_postgame_delay(void)
{
    game_engine_globals.game_ending_timer = 7.0f;
    game_engine_globals.mode = game_engine_mode_postgame_delay;
    game_engine_play_multiplayer_sound(_multiplayer_sound_game_over, 0);
    ui_widgets_close_all();
}
