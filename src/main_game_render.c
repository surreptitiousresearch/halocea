/* main_game_render @0x8368C408 — render one frame of the in-game view: pick the set of player windows
 * (1..2, forced to 1 for single-screen/coop/cinematic), assign each its follow-player camera, append the
 * console overlay window (with a fixed 80-degree-horizontal FOV camera looking down the global axes), then
 * either capture a screenshot or submit the frame to render_frame.
 *
 * Deviations:
 *   - Two raw window-array offsets are &window_0[i].rasterizer_camera.viewport_bounds / .window_bounds
 *     (render_camera @+88, viewport_bounds @+44, window_bounds @+52); re-expressed against the typed
 *     render_window rather than as raw offsets.
 *   - The console-window FOV math used PPC long-double (double-double) register pairs; the high half holding
 *     1.0 is the atan2 denominator. Reproduced as atan2(tan(h_half)*aspect, 1.0).
 *   - 0x3FE6571840000000 is the half horizontal FOV in radians (~40 deg); written as the literal. */

#include <stdint.h>
#include "headers/render_window.h"
#include "headers/observer_result.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/main_globals.h"
#include "headers/bitmap_data.h"
#include "headers/game_connection.h"
#include <math.h>
#include <string.h>
#include "headers/auxbutton.h"
#include "headers/blam_data_globals.h"


#include "headers/point2d.h"
#include "headers/bitmap_data.h"
extern void hcex_pix_begin_event(const char *fmt, ...); /* DEVIATION: variadic (std r4..r10 save-area prologue @0x823CED10) */
extern void hcex_pix_end_event(void);
extern void lock_global_random_seed(void);
extern void unlock_global_random_seed(void);
extern uint8_t game_engine_force_single_screen(void);
extern int16_t local_player_count(void);
extern uint8_t cinematic_in_progress(void);
extern int16_t local_player_get_next(int16_t local_player_index);
extern const observer_result *observer_get_camera(int16_t local_player_index);
extern void compute_window_bounds(int player_index, int num_players, rectangle2d *pixel_bounds, rectangle2d *safe_frame_bounds);
extern void set_window_camera_values(struct render_window *current_window, const observer_result *observer);
extern void render_frame(const struct render_window *windoze, int16_t window_count, const point2d *screenshot_page_index, const point2d *screenshot_index, bitmap_data *screenshot_bitmap, float time_delta_since_tick_sec, float time_delta_since_frame_sec);
extern void screenshot_render(struct render_window *window);
extern uint8_t input_abstraction_auxbutton_down(int16_t button);

void main_game_render(float time_delta_since_tick_sec, float time_delta_since_last_render_sec)
{
    hcex_pix_begin_event("main_game_render");
    lock_global_random_seed();

    int16_t forced_player_index = -1;
    unsigned char single_screen = game_engine_force_single_screen();
    char force_single_player = 0;
    int16_t last_assigned_player = -1;

    int window_count;
    if ( local_player_count() >= 1 )
        window_count = local_player_count() <= 2 ? local_player_count() : 2;
    else
        window_count = 1;

    if ( hcex_coop_local_player_index >= 0 )
    {
        forced_player_index = hcex_coop_local_player_index;
        force_single_player = 1;
    }

    if ( single_screen || force_single_player || cinematic_in_progress() )
        window_count = 1;

    int window_index = 0;
    if ( window_count > 0 )
    {
        struct render_window *window = window_0;
        do
        {
            const observer_result *camera = 0;
            compute_window_bounds(window_index, window_count,
                                  &window->rasterizer_camera.viewport_bounds,
                                  &window->rasterizer_camera.window_bounds);

            if ( single_screen || window_index >= window_count )
            {
                window->local_player_index = -1;
            }
            else if ( rasterizer_debug_options.force_all_player_views_to_default_player && last_assigned_player != -1 )
            {
                window->local_player_index = last_assigned_player;
                camera = observer_get_camera(last_assigned_player);
            }
            else if ( main_globals.connection == _game_connection_film_playback )
            {
                window->local_player_index = 0;
                last_assigned_player = 0;
                camera = observer_get_camera(last_assigned_player);
            }
            else if ( force_single_player && forced_player_index != -1 )
            {
                window->local_player_index = forced_player_index;
                last_assigned_player = forced_player_index;
                camera = observer_get_camera(forced_player_index);
            }
            else
            {
                last_assigned_player = local_player_get_next(last_assigned_player);
                window->local_player_index = last_assigned_player;
                camera = observer_get_camera(last_assigned_player);
            }

            set_window_camera_values(window, camera);
            ++window_index;
            window->console_window = 0;
            ++window;
        }
        while ( window_index < window_count );
    }

    struct render_window *console_window = &window_0[window_count];
    compute_window_bounds(0, 1,
                          &console_window->rasterizer_camera.viewport_bounds,
                          &console_window->rasterizer_camera.window_bounds);
    console_window->local_player_index = -1;
    console_window->console_window = 1;

    console_window->rasterizer_camera.position = *global_origin3d;
    console_window->rasterizer_camera.forward = *global_forward3d;
    console_window->rasterizer_camera.up = *global_up3d;
    console_window->rasterizer_camera.mirrored = 0;

    double half_horizontal_fov = 0.69813168;   /* ~40 deg, raw 0x3FE6571840000000 */
    double vertical_half = atan2(((float)tan(half_horizontal_fov) * 0.75f), 1.0);
    console_window->rasterizer_camera.z_near = rasterizer_globals.z_near;
    console_window->rasterizer_camera.z_far = rasterizer_globals.z_far;
    console_window->rasterizer_camera.vertical_field_of_view = (float)vertical_half * 2.0f;

    if ( debug_render_freeze == 0 )
        memcpy(&console_window->render_camera, &console_window->rasterizer_camera, sizeof(console_window->render_camera));

    if ( EnabledScreenShot )
    {
        int16_t shot_count;
        if ( input_abstraction_auxbutton_down(_auxbutton_screenshot) )
        {
            shot_count = 1;
            global_screenshot_count = 1;
        }
        else
        {
            shot_count = global_screenshot_count;
        }

        if ( shot_count > 0 )
        {
            screenshot_render(window_0);
            goto done;
        }
    }

    render_frame(window_0, window_count + 1, 0, 0, main_globals.movie,
                 time_delta_since_tick_sec, time_delta_since_last_render_sec);

done:
    /* Deviation: decompiler chained these through r3; both are void no-arg. Unchained. */
    unlock_global_random_seed();
    hcex_pix_end_event();
}
