#include <stdint.h>
#include "headers/blam_data_globals.h"
/* ui_loading_start_closing @0x83764500 — begin (or force) closing the loading screen. When forced
 * ("just go"), the loading-screen state is hard-reset. Otherwise, if a loading screen is up, schedule its
 * close ~1.75s out, padding the delay so the screen stays up for a minimum of 2s (0x7D0) since it opened
 * (unless already in the closing state 1). */

extern uint32_t system_milliseconds(void);

void ui_loading_start_closing(uint8_t why_are_we_always_preparing_just_go)
{
    if ( why_are_we_always_preparing_just_go )
    {
        loading_screen_close_time_ms = -1;
        loading_screen_open_time_ms = -1;
        loading_screen_retry_count = 0;
        loading_screen_state = 0;
        loading_screen_connect_ip[0] = 0;
        loading_screen_map[0] = 0;
        loading_screen_natneg_cookie = -1;
    }
    else if ( loading_screen_state )
    {
        unsigned int now = system_milliseconds();
        unsigned int pad = 0;
        if ( loading_screen_open_time_ms != -1 && now - loading_screen_open_time_ms < 0x7D0 && loading_screen_state != 1 )
        {
            pad = loading_screen_open_time_ms - now + 2000;
            if ( pad > 0x7D0 )
                pad = 2000;
        }
        loading_screen_close_time_ms = pad + now + 1750;
    }
}
