/* scripted_hud_set_timer_time @0x836A4178 — start the HUD countdown timer at the given minutes/seconds
 * (30 ticks per second), unpaused and enabled, anchored at the current game time, with the corner re-clamped
 * to 0..4. */

#include <stdint.h>
#include "headers/hud_messaging_globals.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);

void scripted_hud_set_timer_time(int16_t minutes, int16_t seconds)
{
    hud_messaging_globals->timer.paused = 0;
    hud_messaging_globals->timer.enabled = 1;
    hud_messaging_globals->timer.ticks = 30 * (60 * minutes + seconds);
    hud_messaging_globals->timer.reference_time = game_time_get();
    int16_t corner = hud_messaging_globals->timer.corner;
    if ( corner < 0 )
        corner = 0;
    else if ( corner > 4 )
        corner = 4;
    hud_messaging_globals->timer.corner = corner;
}
