/* game_dispose @ 0x83687848 — one-time teardown of every Blam subsystem.
 * The decompiler threads a status value through r3 across the calls; it is
 * unused, so the calls are written as a plain sequence. */

extern void recorded_animations_dispose(void);
extern void cinematic_dispose(void);
extern void hs_dispose(void);
extern void cheats_dispose(void);
extern void ui_widgets_dispose(void);
extern void editor_dispose(void);
extern void ai_dispose(void);
extern void player_effect_dispose(void);
extern void vibrate_dispose(void);
extern void game_sound_dispose(void);
extern void sound_classes_dispose(void);
extern void game_engine_dispose(void);
extern void particle_systems_dispose(void);
extern void weather_particle_systems_dispose(void);
extern void effects_dispose(void);
extern void particles_dispose(void);
extern void contrails_dispose(void);
extern void players_dispose(void);
extern void decals_dispose(void);
extern void breakable_surfaces_dispose(void);
extern void structures_dispose(void); /* attested void */
extern void render_dispose(void);
extern void objects_dispose(void);
extern void director_dispose(void);
extern void interface_dispose(void);
extern void game_allegiance_dispose(void);
extern void game_time_dispose(void);
extern void save_film_dispose(void);
extern void saved_game_files_dispose(void);
extern void event_manager_dispose(void);
extern void input_abstraction_dispose(void);
extern void player_ui_dispose(void);
extern void game_state_dispose(void);
extern void bink_playback_dispose(void);
extern void progress_bar_dispose(void);

void game_dispose(void) /* attested void: 0/2 callers consume r3 */
{
    recorded_animations_dispose(); /* attested void */
    cinematic_dispose();
    hs_dispose();
    cheats_dispose();
    ui_widgets_dispose();
    editor_dispose();
    ai_dispose();
    player_effect_dispose();
    vibrate_dispose();
    game_sound_dispose();
    sound_classes_dispose();
    game_engine_dispose();
    particle_systems_dispose();
    weather_particle_systems_dispose();
    effects_dispose();
    particles_dispose();
    contrails_dispose();
    players_dispose();
    decals_dispose();
    breakable_surfaces_dispose();
    structures_dispose();
    render_dispose();
    objects_dispose();
    director_dispose();
    interface_dispose();
    game_allegiance_dispose();
    game_time_dispose();
    save_film_dispose();
    saved_game_files_dispose();
    event_manager_dispose();
    input_abstraction_dispose(); /* attested void */
    player_ui_dispose();
    game_state_dispose();
    bink_playback_dispose(); /* attested void */
    progress_bar_dispose(); /* attested void */
}
