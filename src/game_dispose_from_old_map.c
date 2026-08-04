/* game_dispose_from_old_map @ 0x83687CD0 — per-map teardown of every subsystem.
 * Status value threaded through r3 is unused; written as a plain sequence. */

#include <stdint.h>
#include "headers/game_globals.h"

extern void rasterizer_dispose_from_old_map(void); /* attested void */
extern void game_state_dispose_from_old_map(void);
extern void cheats_dispose_from_old_map(void);
extern void recorded_animations_dispose_from_old_map(void);
extern void hs_dispose_from_old_map(void); /* attested void */
extern void cinematic_dispose_from_old_map(void);
extern void editor_dispose_from_old_map(void);
extern void ai_dispose_from_old_map(void);
extern void player_effect_dispose_from_old_map(void);
extern void vibrate_dispose_from_old_map(void);
extern void point_physics_dispose_from_old_map(void);
extern void particle_systems_dispose_from_old_map(void);
extern void weather_particle_systems_dispose_from_old_map(void);
extern void decals_dispose_from_old_map(void);
extern void breakable_surfaces_dispose_from_old_map(void);
extern void structures_dispose_from_old_map(void); /* attested void */
extern void render_dispose_from_old_map(void);
extern void objects_dispose_from_old_map(void);
extern void director_dispose_from_old_map(void);
extern void observer_dispose_from_old_map(void);
extern void interface_dispose_from_old_map(void);
extern void players_dispose_from_old_map(void);
extern void contrails_dispose_from_old_map(void);
extern void particles_dispose_from_old_map(void);
extern void effects_dispose_from_old_map(void);
extern void game_sound_dispose_from_old_map(void);
extern void sound_classes_dispose_from_old_map(void);
extern void sound_dispose_from_old_map(void);
extern void game_allegiance_dispose_from_old_map(void);
extern void update_server_delete(void);
extern void game_engine_dispose_from_old_map(void);
extern void scenario_dispose_from_old_map(void);
extern void game_time_dispose_from_old_map(void);
extern void save_film_end_recording(void);
extern void ui_widgets_close_all(void);
extern void ui_widgets_safe_to_load(uint8_t safe);

void game_dispose_from_old_map(void)
{
    rasterizer_dispose_from_old_map(); /* attested void */
    game_state_dispose_from_old_map();
    cheats_dispose_from_old_map();
    recorded_animations_dispose_from_old_map();
    hs_dispose_from_old_map();
    cinematic_dispose_from_old_map();
    editor_dispose_from_old_map();
    ai_dispose_from_old_map();
    player_effect_dispose_from_old_map();
    vibrate_dispose_from_old_map();
    point_physics_dispose_from_old_map();
    particle_systems_dispose_from_old_map();
    weather_particle_systems_dispose_from_old_map();
    decals_dispose_from_old_map();
    breakable_surfaces_dispose_from_old_map();
    structures_dispose_from_old_map(); /* attested void */
    render_dispose_from_old_map();
    objects_dispose_from_old_map();
    director_dispose_from_old_map();
    observer_dispose_from_old_map();
    interface_dispose_from_old_map();
    players_dispose_from_old_map();
    contrails_dispose_from_old_map();
    particles_dispose_from_old_map();
    effects_dispose_from_old_map();
    game_sound_dispose_from_old_map();
    sound_classes_dispose_from_old_map();
    sound_dispose_from_old_map();
    game_allegiance_dispose_from_old_map();
    update_server_delete();
    game_engine_dispose_from_old_map();
    scenario_dispose_from_old_map(); /* attested void */
    game_time_dispose_from_old_map();
    save_film_end_recording();
    ui_widgets_close_all();
    ui_widgets_safe_to_load(0);
    game_globals->active = 0;
}
