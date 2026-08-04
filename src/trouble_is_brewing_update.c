#include <stdint.h>
#include "headers/blam_data_globals.h"
/* trouble_is_brewing_update @0x83734334 — per-frame watchdog: if "trouble" (a hang/long-frame
 * condition) was flagged this frame, show the warning dialog once 10 seconds (0x2710 ms) have elapsed
 * since it first started and the dialog isn't already up; otherwise reset the timer. */

extern uint32_t system_milliseconds(void);

void trouble_is_brewing_update(void)
{
    if ( trouble_brewed_this_frame )
    {
        if ( trouble_is_brewing_time == -1
          || trouble_brewing_dialog_visible
          || (int)(system_milliseconds() - trouble_is_brewing_time) < 0x2710 )
        {
            trouble_brewed_this_frame = 0;
        }
        else
        {
            trouble_brewing_dialog_visible = 1;
            trouble_brewed_this_frame = 0;
        }
    }
    else
    {
        trouble_brewing_dialog_visible = 0;
        trouble_is_brewing_time = -1;
        trouble_brewed_this_frame = 0;
    }
}
