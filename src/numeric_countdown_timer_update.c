#include "headers/blam_data_globals.h"
/* numeric_countdown_timer_update @0x83755ED8 — when the on-screen numeric countdown timer is running,
 * decrements its remaining milliseconds by the real game-time that has elapsed since the last frame
 * (game ticks converted to ms: ticks * 1000 / 30), clamped at zero. */


extern int game_time_get(void);

void numeric_countdown_timer_update(void)
{
    if ( numeric_countdown_timer_on )
    {
        int now_ms = 1000 * game_time_get() / 30;
        if ( now_ms >= previous_game_time )
        {
            numeric_countdown_timer_milliseconds += previous_game_time - now_ms;
            if ( numeric_countdown_timer_milliseconds < 0 )
                numeric_countdown_timer_milliseconds = 0;
        }
        previous_game_time = now_ms;
    }
}
