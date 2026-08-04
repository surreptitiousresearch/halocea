/* sound_pause @ 0x83715170 — pause/resume the sound system (idempotent). On resume, the
 * render-time clock is rebased so paused time doesn't accumulate. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"

extern uint32_t main_get_mseconds(void);

void sound_pause(uint8_t paused)
{
    if ( paused != sound_manager_globals.paused )
    {
        sound_manager_globals.paused = paused;
        if ( sound_manager_globals.platform )
            sound_manager_globals.platform->set_pause(paused);
        if ( !paused )
            sound_manager_globals.render_time = main_get_mseconds();
    }
}
