/* render_window @0x83707268 — render one player window: set up render-globals + the rasterizer window
 * parameters from the supplied render/rasterizer cameras and frusta, compute visibility, then drive the full
 * scene render pipeline in order (sky, first-person weapon, lights, objects, structure passes, decals,
 * reflections, transparents, particles, contrails, detail objects, fog, lens flares, interface, screen flash).
 * When a Bink movie is playing the scene pipeline is skipped (only ambient is disabled and the movie drawn).
 *
 * The long `v15 = f(); v16 = g(v15); ...` chain is a decompiler artifact: every one of these render-stage
 * functions is void(void) and the apparent r3 threading is just the leftover return register (confirmed: the
 * empty DB prototypes and structure_visibility_compute reuses r3 without reading it). Written as bare calls. */

#include <stdint.h>
#include <string.h>
#include "headers/render_globals.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/render_camera.h"
#include "headers/render_frustum.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/real_vector3d.h"
#include "headers/decal_layer.h"
#include "headers/blam_data_globals.h"


#include "headers/render_screen_flash.h"
/* memcpy declared by <string.h> */

extern void structure_visibility_compute(void);
extern void player_effect_get_screen_flash(int16_t local_player_index, render_screen_flash *screen_flash);
extern void rasterizer_window_begin(const rasterizer_window_begin_parameters *parameters);
extern void rasterizer_window_end(void);
extern uint8_t bink_playback_in_progress(void);
extern void bink_playback_render(void);
extern void set_effect_ambient(uint8_t in_game_play);
extern void rasterizer_decals_begin(int16_t layer);
extern void rasterizer_decals_draw(int16_t cluster_index);
extern void rasterizer_decals_end(void);
extern void rasterizer_transparent_geometry_draw(uint8_t water);
extern void render_camera_debug_frustum(const render_camera *camera, const render_frustum *frustum);

/* scene-pipeline stages — all void(void) except build_sprite_prepare_for_window (returns its
 * final matrix4x3_transform_normal result, which the caller ignores) */
extern void build_sprite_prepare_for_window(void);
extern void render_sky(void);
extern void first_person_weapon_render_update(void);
extern void lights_preprocess_scene(void);
extern void render_objects(void);
extern void structure_render_preprocess(void);
extern void structure_render_lightmaps(void);
extern void rasterizer_lens_flares_submit_occlusion_tests(void);
extern void render_object_shadows(void);
extern void lights_render_diffuse(void);
extern void structure_render_diffuse_texture(void);
extern void lights_render_specular(void);
extern void structure_render_specular_lightmaps(void);
extern void structure_render_reflection_lightmap_masks(void);
extern void structure_render_reflection_mirrors(void);
extern void structure_render_reflections(void);
extern void structure_render_transparent_geometry(void);
extern void structure_render_fog(void);
extern void game_engine_post_rasterize_objects(void);
extern void weather_particle_systems_render(void);
extern void render_particles(void);
extern void particle_systems_render(void);
extern void render_contrails_normal(void);
extern void structure_render_detail_objects(void);
extern void rasterizer_transparent_geometry_stop(void);
extern void structure_render_fog_screen(void);
extern void rasterizer_lens_flares_draw(void);
extern void interface_draw_screen(void);
extern void rasterizer_screen_flash(void);
extern void editor_render(void);
extern void rasterizer_debug_draw(void);

static void render_window_draw_cluster_decals(void)
{
    for (int cluster = 0; cluster < render.rendered_cluster_count; cluster = (int16_t)(cluster + 1))
        rasterizer_decals_draw(render.rendered_clusters[cluster].cluster_index);
}

/* params renamed from render_camera/render_frustum: those names shadow the
 * typedefs of the same name, breaking the later parameter declarators. */
void render_window(int16_t local_player_index, const render_camera *source_camera, const render_frustum *source_frustum, const render_camera *rasterizer_camera, const render_frustum *rasterizer_frustum, int16_t rasterizer_target, uint8_t has_mirror)
{
    ++render.scene_index;
    render.local_player_index = local_player_index;

    rasterizer_window_begin_parameters parameters;
    memset(&parameters, 0, sizeof(parameters));
    memcpy(&render.camera, source_camera, sizeof(render.camera));
    memcpy(&render.frustum, source_frustum, sizeof(render.frustum));
    memcpy(&parameters.camera, rasterizer_camera, sizeof(parameters.camera));
    memcpy(&parameters.frustum, rasterizer_frustum, sizeof(parameters.frustum));
    parameters.rasterizer_target = rasterizer_target;
    parameters.has_mirror = has_mirror;
    parameters.window_index = render.window_index;
    memcpy(&parameters.fog, &render.fog, sizeof(parameters.fog));
    structure_visibility_compute();
    player_effect_get_screen_flash(local_player_index, &parameters.screen_flash);
    rasterizer_window_begin(&parameters);

    if (bink_playback_in_progress())
    {
        set_effect_ambient(0);
    }
    else
    {
        set_effect_ambient(1);
        build_sprite_prepare_for_window();
        render_sky();
        first_person_weapon_render_update();
        lights_preprocess_scene();
        render_objects();
        structure_render_preprocess();
        structure_render_lightmaps();
        rasterizer_lens_flares_submit_occlusion_tests();
        render_object_shadows();
        lights_render_diffuse();

        if (!rasterizer_debug_options.drawing_mode && rasterizer_debug_options.draw_environment_decals)
        {
            rasterizer_decals_begin(_decal_layer_light);
            render_window_draw_cluster_decals();
            rasterizer_decals_end();
        }

        rasterizer_decals_begin(_decal_layer_alpha_tested);
        render_window_draw_cluster_decals();
        rasterizer_decals_end();
        structure_render_diffuse_texture();

        if (!rasterizer_debug_options.drawing_mode && rasterizer_debug_options.draw_environment_decals)
        {
            rasterizer_decals_begin(_decal_layer_primary);
            render_window_draw_cluster_decals();
            rasterizer_decals_end();
            rasterizer_decals_begin(_decal_layer_secondary);
            render_window_draw_cluster_decals();
            rasterizer_decals_end();
        }

        lights_render_specular();
        structure_render_specular_lightmaps();
        structure_render_reflection_lightmap_masks();
        structure_render_reflection_mirrors();
        structure_render_reflections();
        structure_render_transparent_geometry();
        structure_render_fog();
        game_engine_post_rasterize_objects();
        weather_particle_systems_render();
        render_particles();
        particle_systems_render();
        render_contrails_normal();
        rasterizer_transparent_geometry_draw(1);

        rasterizer_decals_begin(_decal_layer_water);
        render_window_draw_cluster_decals();
        rasterizer_decals_end();
        structure_render_detail_objects();
        rasterizer_transparent_geometry_draw(0);
        rasterizer_transparent_geometry_stop();
        structure_render_fog_screen();
        rasterizer_lens_flares_draw();
        interface_draw_screen();
        rasterizer_screen_flash();
    }

    bink_playback_render();
    render_camera_debug_frustum(&render.camera, &render.frustum);
    editor_render();
    rasterizer_debug_draw();
    rasterizer_window_end();
}
