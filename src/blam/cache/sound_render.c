/* sound_render @ 0x8371AF58 — the per-frame sound tick. Updates the global pause state, and (when the
 * mixer is initialised, active and not disabled) advances the sound clock; if at least ~33ms (0x21
 * ticks) have elapsed it brackets a platform render scene and, unless paused, runs the full mixer
 * pipeline: class gain smoothing -> listener refresh -> looping-sound advance -> sound refresh ->
 * prioritisation -> channel update, toggling the double-buffer flip-flop. The cache idle pass runs
 * whenever not paused.
 *
 * Deviation: the decompiler threaded a register (`updated`) through every call as if each took/
 * returned it; the prototypes (and the platform begin_scene/end_scene) are all void — the threading
 * is register-reuse noise and is dropped here. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"

extern uint8_t game_time_get_paused(void);
extern uint8_t console_is_active(void);
extern uint8_t game_engine_running(void);
extern void sound_pause(uint8_t paused);
extern void sound_update_time(void);
extern void sound_classes_update(int ticks_elapsed);
extern void refresh_listener(void);
extern void process_looping_sounds(void);
extern void refresh_sounds(void);
extern void prioritize_sounds(void);
extern void update_channels(void);
extern void sound_cache_idle(void);

void sound_render(void)
{
    unsigned char paused;

    if ( sound_manager_globals.disable_from_commandline )
        return;

    paused = game_time_get_paused() || (console_is_active() && !game_engine_running());
    sound_pause(paused);

    if ( sound_manager_globals.initialized && sound_manager_globals.active
      && !sound_manager_globals.disable_from_commandline )
    {
        int previous_render_time = sound_manager_globals.render_time;
        float previous_ticks_elapsed = sound_manager_globals.ticks_elapsed;
        char scene_active = 1;

        sound_update_time();
        if ( (unsigned int)(sound_manager_globals.render_time - previous_render_time) < 0x21 )
            scene_active = 0;
        else
            sound_manager_globals.platform->begin_scene();

        if ( sound_manager_globals.paused || !scene_active )
        {
            /* not enough time advanced (or paused): roll the clock back */
            sound_manager_globals.ticks_elapsed = previous_ticks_elapsed;
            sound_manager_globals.render_time = previous_render_time;
        }
        else
        {
            sound_classes_update((int)sound_manager_globals.ticks_elapsed);
            refresh_listener();
            process_looping_sounds();
            refresh_sounds();
            prioritize_sounds();
            update_channels();
            sound_manager_globals.flip_flop = !sound_manager_globals.flip_flop;
        }

        if ( scene_active )
            sound_manager_globals.platform->end_scene();
    }

    if ( !sound_manager_globals.paused )
        sound_cache_idle();
}
