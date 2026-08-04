/* sound_idle @ 0x8371AAB0 — service the sound system outside the normal frame loop (e.g. while
 * blocking on cache loads). Advances the sound clock and, if at least ~33ms have elapsed, runs a
 * mix scene (begin/update channels/end); restores the clock when paused or too soon. Always pumps
 * the sound cache. Takes no args and returns void (disasm: r3 untouched at entry, no r3 at blr). */

#include "headers/sound_manager_globals.h"

extern void sound_update_time(void);
extern void update_channels(void);
extern void sound_cache_idle(void);

void sound_idle(void)
{
    sound_manager_globals.idling = 1;

    if ( sound_manager_globals.initialized
      && sound_manager_globals.active
      && !sound_manager_globals.disable_from_commandline )
    {
        int saved_render_time = sound_manager_globals.render_time;
        float saved_ticks_elapsed = sound_manager_globals.ticks_elapsed;
        char run_scene = 1;

        sound_update_time();
        if ( (unsigned int)(sound_manager_globals.render_time - saved_render_time) < 0x21 )
            run_scene = 0;
        else
            sound_manager_globals.platform->begin_scene();

        if ( sound_manager_globals.paused || !run_scene )
        {
            sound_manager_globals.ticks_elapsed = saved_ticks_elapsed;
            sound_manager_globals.render_time = saved_render_time;
        }
        else
        {
            update_channels();
        }

        if ( run_scene )
            sound_manager_globals.platform->end_scene();
    }

    sound_cache_idle();
    sound_manager_globals.idling = 0;
}
