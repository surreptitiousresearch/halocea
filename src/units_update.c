/* units_update @ 0x836C6C50 — per-frame reset of the unit update-timer budget. Each frame the highest
 * timer reached last frame becomes this frame's starting timer, and the running accumulators are
 * cleared. The actual unit work happens in objects_update; this just rolls the timer accounting. */

#include "headers/unit_globals.h"

void units_update(void)
{
    __int16 highest_timer = unit_globals->highest_timer;
    unit_globals->highest_timer = 0;
    unit_globals->used_time = 0;
    unit_globals->next_timer = highest_timer;
}
