/* game_initialize_for_new_map @0x83687B78 — the per-map initialization entry point: seeds the global random
 * number generator from the map's saved seed, begins saved-film recording (unless this is the UI map), then
 * runs every subsystem's _initialize_for_new_map in order before placing scenario objects and AI.
 *
 * Deviation: the decompiler threads each subsystem call's return value into the next as a spurious argument
 * (register reuse); the calls are all void and reproduced as plain statements. The map-name check is an inlined
 * strcmp against the UI level, restored to strcmp. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/game_globals.h"
#include "headers/game_variant.h"
extern int strcmp(const char *string1, const char *string2);

extern uint32_t *get_global_random_seed_address(void);

extern int hcex_obj_clear(void);
extern void player_ui_initialize_for_new_map(void);
extern void save_film_begin_recording(void);
extern void game_engine_dispose(void);
extern void game_engine_initialize(const game_variant *variant);
extern void real_math_reset_precision(void);
extern void rasterizer_initialize_for_new_map(void);
extern void game_state_initialize_for_new_map(void);
extern void game_time_initialize_for_new_map(void);
extern void interface_initialize_for_new_map(void);
extern void game_allegiance_initialize_for_new_map(void);
extern void players_initialize_for_new_map(void);
extern void scenario_initialize_for_new_map(void);
extern void objects_initialize_for_new_map(void);
extern void render_initialize_for_new_map(void);
extern void structures_initialize_for_new_map(void);
extern void breakable_surfaces_initialize_for_new_map(void);
extern void decals_initialize_for_new_map(void);
extern void director_initialize_for_new_map(void);
extern void observer_initialize_for_new_map(void);
extern void contrails_initialize_for_new_map(void);
extern void particles_initialize_for_new_map(void);
extern void effects_initialize_for_new_map(void);
extern void particle_systems_initialize_for_new_map(void);
extern void sound_initialize_for_new_map(void);
extern void sound_classes_initialize_for_new_map(void);
extern void game_sound_initialize_for_new_map(void);
extern void weather_particle_systems_initialize_for_new_map(void);
extern void point_physics_initialize_for_new_map(void);
extern void game_engine_initialize_for_new_map(void);
extern void game_statistics_start(void);
extern uint8_t update_server_new(void);
extern void player_control_initialize_for_new_map(void);
extern void vibrate_initialize_for_new_map(void);
extern void player_effect_initialize_for_new_map(void);
extern void ai_initialize_for_new_map(void);
extern void console_initialize_for_new_map(void);
extern void editor_initialize_for_new_map(void);
extern void cinematic_initialize_for_new_map(void);
extern void hs_initialize_for_new_map(void);
extern void recorded_animations_initialize_for_new_map(void);
extern void cheats_initialize_for_new_map(void);
extern void objects_place(void);
extern uint8_t game_in_editor(void);
extern void ai_place(void);
extern void ui_widgets_safe_to_load(uint8_t safe);

void game_initialize_for_new_map(void)
{
    hcex_obj_clear();
    player_ui_initialize_for_new_map();

    /* seed the global RNG from the map's saved seed */
    unsigned int random_seed = game_globals->options.random_seed;
    *get_global_random_seed_address() = random_seed;

    /* record a saved film for any level other than the front-end UI map */
    if ( strcmp(game_globals->options.map_name, "levels\\ui\\ui") != 0 )
        save_film_begin_recording();

    game_engine_dispose();
    game_engine_initialize(&game_variant_global);

    real_math_reset_precision();
    rasterizer_initialize_for_new_map();
    game_state_initialize_for_new_map();
    game_time_initialize_for_new_map();
    interface_initialize_for_new_map();
    game_allegiance_initialize_for_new_map();
    players_initialize_for_new_map();
    scenario_initialize_for_new_map();
    objects_initialize_for_new_map();
    render_initialize_for_new_map();
    structures_initialize_for_new_map();
    breakable_surfaces_initialize_for_new_map();
    decals_initialize_for_new_map();
    director_initialize_for_new_map();
    observer_initialize_for_new_map();
    contrails_initialize_for_new_map();
    particles_initialize_for_new_map();
    effects_initialize_for_new_map();
    particle_systems_initialize_for_new_map();
    sound_initialize_for_new_map();
    sound_classes_initialize_for_new_map();
    game_sound_initialize_for_new_map();
    weather_particle_systems_initialize_for_new_map();
    point_physics_initialize_for_new_map();
    game_engine_initialize_for_new_map();
    game_statistics_start();
    update_server_new();
    player_control_initialize_for_new_map();
    vibrate_initialize_for_new_map();
    player_effect_initialize_for_new_map();
    ai_initialize_for_new_map();
    console_initialize_for_new_map();
    editor_initialize_for_new_map();
    cinematic_initialize_for_new_map();
    hs_initialize_for_new_map();
    recorded_animations_initialize_for_new_map();
    cheats_initialize_for_new_map();

    game_globals->active = 1;
    objects_place();
    if ( !game_in_editor() )
        ai_place();
    ui_widgets_safe_to_load(1u);
}
