/* render_frame_pregame @0x83707644 — render one pre-game frame (loading screen / Bink intro). Begins the
 * rasterizer frame and windows, builds the render and rasterizer frustums from the window's cameras, renders
 * the UI widgets, any Bink video, and the loading progress bar, then ends the windows and frame.
 * DEVIATION: ui_loading_update_and_render / rasterizer_window_end / rasterizer_windows_end /
 * rasterizer_frame_end form a threaded-r3 chain of independent no-arg calls. screenshot_bitmap is unused. */

#include <stdint.h>
#include "headers/render_window.h"
#include "headers/render_globals.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/bitmap_data.h"
#include "headers/rectangle2d.h"
#include "headers/rasterizer_target.h"

typedef struct rasterizer_frame_begin_parameters rasterizer_frame_begin_parameters;

#include "headers/real_rectangle2d.h"
#include "headers/render_frustum.h"
#include "headers/rectangle2d.h"
extern void *memset(void *dst, int value, unsigned int count);
extern void *memcpy(void *dst, const void *src, unsigned int count);
extern void rasterizer_frame_begin(const rasterizer_frame_begin_parameters *parameters);
extern uint8_t rasterizer_windows_begin(void);
extern void render_camera_build_frustum(const render_camera *camera, const real_rectangle2d *frustum_bounds, render_frustum *frustum, uint8_t build_projection);
extern void rasterizer_window_begin(const rasterizer_window_begin_parameters *parameters);
extern void render_ui_widgets(int16_t local_player_index, const rectangle2d *window_bounds);
extern void bink_playback_render(void);
extern uint8_t game_map_loading_in_progress(float *progress);
extern void progress_bar_display(float progress);
extern void ui_loading_update_and_render(void);
extern void rasterizer_window_end(void);
extern void rasterizer_windows_end(void);
extern void rasterizer_frame_end(void);

void render_frame_pregame(const struct render_window *window, bitmap_data *screenshot_bitmap)
{
    ++render.frame_index;

    char frame_begin_parameters[16];
    rasterizer_frame_begin((const rasterizer_frame_begin_parameters *)frame_begin_parameters);
    if ( rasterizer_windows_begin() )
    {
        rasterizer_window_begin_parameters window_parameters;
        memset(&window_parameters, 0, sizeof(window_parameters));
        memcpy(&render.camera, &window->render_camera, sizeof(render.camera));
        render_camera_build_frustum(&render.camera, 0, &render.frustum, 1u);
        memcpy(&window_parameters.camera, &window->rasterizer_camera, sizeof(window_parameters.camera));
        render_camera_build_frustum(&window_parameters.camera, 0, &window_parameters.frustum, 1u);
        window_parameters.rasterizer_target = _rasterizer_target_render_primary;
        rasterizer_window_begin(&window_parameters);
        render_ui_widgets(0, &window->rasterizer_camera.viewport_bounds);
        bink_playback_render();

        float progress[4];
        if ( game_map_loading_in_progress(progress) )
            progress_bar_display(progress[0]);
        ui_loading_update_and_render();
        rasterizer_window_end();
        rasterizer_windows_end();
        rasterizer_frame_end();
    }
}
