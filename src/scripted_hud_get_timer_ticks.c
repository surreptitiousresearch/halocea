/* scripted_hud_get_timer_ticks @0x836A4330 — return the HUD timer's remaining ticks: 0 if disabled, the
 * stored value if paused (or already expired), otherwise the game-time-adjusted remaining count. */

#include <stdint.h>
#include "headers/hud_messaging_globals.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);

int16_t scripted_hud_get_timer_ticks(void)
{
    if ( !hud_messaging_globals->timer.enabled )
        return 0;
    int ticks = hud_messaging_globals->timer.ticks;
    if ( ticks != -1 && !hud_messaging_globals->timer.paused )
    {
        hud_timer_data_definition *timer = &hud_messaging_globals->timer;
        return (int16_t)(timer->ticks + timer->reference_time - game_time_get());
    }
    return (int16_t)ticks;
}
