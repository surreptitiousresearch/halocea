/* sound_render_time @ 0x837151E0 — the sound system's current render-time clock (ms). */

#include "headers/sound_manager_globals.h"

int sound_render_time(void)
{
    return sound_manager_globals.render_time;
}
