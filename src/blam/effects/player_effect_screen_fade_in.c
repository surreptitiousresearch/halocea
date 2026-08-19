/* player_effect_screen_fade_in @0x837379E0 — start a full-screen color fade-IN over `ticks` ticks,
 * timestamped to the current game time.
 *
 * DEVIATION: the decompiler invents extra integer params (a5/a6/a7) from the soft-float GPR-skip; the
 * real prototype is (float red, float green, float blue, short ticks). */

#include <stdint.h>
#include "headers/player_effect_globals.h"

extern int game_time_get(void);

void player_effect_screen_fade_in(float red, float green, float blue, int16_t ticks)
{
    player_effect_globals->screen_fade.color.red = red;
    player_effect_globals->screen_fade.color.green = green;
    player_effect_globals->screen_fade.color.blue = blue;
    player_effect_globals->screen_fade.ticks = ticks;
    player_effect_globals->screen_fade.fading_out = 0;
    player_effect_globals->screen_fade.start_time = game_time_get();
}
