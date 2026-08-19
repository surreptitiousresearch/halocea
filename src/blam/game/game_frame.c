/* game_frame @ 0x83687D90 — the per-render-frame (non-tick) update. Runs the continuous, frame-rate
 * dependent systems: particles, contrails, particle systems, widgets, sound, scenario, rasterizer and
 * the numeric countdown timer. The frame delta is halved when the game runs at double speed.
 *
 * Two distinct deltas are produced: `frame_dt` is the raw (speed-scaled) frame delta passed to the
 * continuous renderers; `tick_dt` converts the number of game ticks elapsed this frame into seconds
 * (ticks * 1/30 s, speed-scaled) and drives the tick-coupled systems (particles, widgets).
 *
 * Deviation: the decompiler modelled the elapsed-tick count through an __int64 (v6); it is a plain
 * int16_t tick count from game_time_get_elapsed().
 * Deviation: Hex-Rays widened the frame-delta param to double; the caller passes an fmuls
 * single-precision result (game_time_update @0x8369AD78/AD90), so dt is float. */

#include <stdint.h>
#include "headers/game_globals.h"
#include "headers/game_time_constants.h"

extern int16_t game_time_get_elapsed(void);
extern void particles_update(float dt);
extern void contrails_update(float dt);
extern void particle_systems_update(float dtime);
extern void widgets_update(float dt);
extern void game_sound_update(float dt);
extern void scenario_frame_update(float dt);
extern void rasterizer_frame_update(float dt);
extern void numeric_countdown_timer_update(void);

void game_frame(float dt)
{
    float speed_scale = game_globals->players_are_double_speed ? 0.5f : 1.0f;
    int16_t elapsed_ticks = game_time_get_elapsed();
    float frame_dt = speed_scale * dt;
    float tick_dt = ((float)elapsed_ticks * speed_scale) * SECONDS_PER_TICK;

    if ( elapsed_ticks )
        particles_update(tick_dt);
    contrails_update(frame_dt);
    particle_systems_update(frame_dt);
    if ( elapsed_ticks )
        widgets_update(tick_dt);
    game_sound_update(frame_dt);
    scenario_frame_update(frame_dt);
    rasterizer_frame_update(frame_dt);
    numeric_countdown_timer_update();
}
