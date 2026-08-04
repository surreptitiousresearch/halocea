/* game_initialize @ 0x83687720 — one-time allocation of every Blam subsystem.
 * Status value threaded through r3 is unused; written as a plain sequence. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include "headers/blam_data_globals.h"
#include "headers/game_globals.h"

#include "headers/game_variant.h"
extern void *game_state_malloc(const char *name, const char *type, int size);
extern void *memset(void *, int, unsigned int);
extern void real_math_reset_precision(void);
extern void game_time_initialize(void);
extern void game_engine_initialize(const game_variant *variant);
extern void game_allegiance_initialize(void);
extern void interface_initialize(void);
extern void scenario_initialize(void);
extern void director_initialize(void);
extern void observer_initialize(void);
extern void render_initialize(void);
extern void objects_initialize(void);
extern void structures_initialize(void);
extern void breakable_surfaces_initialize(void);
extern void decals_initialize(void);
extern void players_initialize(void);
extern void contrails_initialize(void);
extern void particles_initialize(void);
extern void effects_initialize(void);
extern void weather_particle_systems_initialize(void);
extern void particle_systems_initialize(void);
extern void sound_classes_initialize(void);
extern void game_sound_initialize(void);
extern void vibrate_initialize(void);
extern void player_effect_initialize(void);
extern void ai_initialize(void);
extern void editor_initialize(void);
extern void ui_widgets_initialize(void);
extern void hs_initialize(void);
extern void recorded_animations_initialize(void);
extern void cheats_initialize(void);
extern void save_film_initialize(void);
extern void cinematic_initialize(void);
extern void saved_game_files_initialize(void);
extern void event_manager_initialize(void);
extern void input_abstraction_initialize(void);
extern void player_ui_initialize(void);
extern void bink_playback_initialize(void);
extern void progress_bar_initialize(void);


void game_initialize(void)
{
    game_globals = game_state_malloc("game globals", 0, 276);
    memset(game_globals, 0, sizeof(struct_game_globals));
    memset(&game_variant_global, 0, sizeof(game_variant_global));
    real_math_reset_precision(); /* attested void(void): r3-thread arg removed */
    game_time_initialize();
    game_engine_initialize(&game_variant_global);
    game_allegiance_initialize();
    interface_initialize();
    scenario_initialize();
    director_initialize();
    observer_initialize();
    render_initialize();
    objects_initialize();
    structures_initialize();
    breakable_surfaces_initialize();
    decals_initialize();
    players_initialize();
    contrails_initialize();
    particles_initialize();
    effects_initialize();
    weather_particle_systems_initialize();
    particle_systems_initialize();
    sound_classes_initialize();
    game_sound_initialize();
    vibrate_initialize();
    player_effect_initialize();
    ai_initialize();
    editor_initialize();
    ui_widgets_initialize();
    hs_initialize();
    recorded_animations_initialize();
    cheats_initialize();
    save_film_initialize();
    cinematic_initialize();
    saved_game_files_initialize();
    event_manager_initialize();
    input_abstraction_initialize();
    player_ui_initialize();
    bink_playback_initialize(); /* attested void */
    progress_bar_initialize(); /* attested void */

    p_achievements_info = game_state_malloc("achievements info", 0, 1024);
    memset(p_achievements_info, 0, sizeof(achievements_info));
}
