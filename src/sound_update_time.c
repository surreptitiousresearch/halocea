/* sound_update_time @ 0x837153D8 — advance the sound clock to the current millisecond count and
 * record the elapsed time in 30Hz ticks (1 ms ≈ 0.03 tick). Returns the new render time. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"

extern uint32_t main_get_mseconds(void);

void sound_update_time(void)
{
    int now = main_get_mseconds();
    int previous = sound_manager_globals.render_time;

    sound_manager_globals.render_time = now;
    sound_manager_globals.ticks_elapsed = (float)(now - previous) * 0.029999999f;
}
