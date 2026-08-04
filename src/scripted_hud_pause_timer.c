/* scripted_hud_pause_timer @0x836A4278 — pause or resume the HUD countdown timer, converting the stored tick
 * count between an absolute-remaining form (paused) and a game-time-anchored form (running).
 *
 * DEVIATION: the decompiler renders reference_time through HIWORD(); the field is a 32-bit game time and is
 * used whole here. */

#include <stdint.h>
#include "headers/hud_messaging_globals.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);

void scripted_hud_pause_timer(uint8_t paused)
{
    hud_timer_data_definition *timer = &hud_messaging_globals->timer;
    __int16 ticks = timer->ticks;
    timer->paused = paused;
    if ( ticks > 0 )
    {
        if ( paused )
            timer->ticks += timer->reference_time - game_time_get();
        else
            timer->ticks = game_time_get() + timer->ticks - timer->reference_time;
    }
}
