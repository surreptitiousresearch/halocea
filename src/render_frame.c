/* render_frame @0x83707A40 — render one display frame across all render windows. Stamps the per-frame
 * time deltas, opens the rasterizer frame, then for each window dispatches by kind: console windows draw
 * only the fullscreen interface overlays and debug geometry; player windows render the full player view
 * (optionally tagged with a screenshot tile index); other windows run the non-player post-rasterize pass.
 * Finally it ticks the progress bar and closes the windows and the frame.
 *
 * DEVIATION: the decompiler's `__PAIR64__("glow_convolve", game_time_get())` is just the PPC fcfid
 * int->double conversion of game_time_get() (the string is coincidental register reuse). It also threads
 * return values between the trailing void render calls (interface_draw_fullscreen_overlays ->
 * rasterizer_debug_draw -> rasterizer_window_end, etc.); the disassembly shows these are bare calls with
 * no arguments, so they are written as void calls here. */

#include <stdint.h>
#include "headers/render_window.h"
#include "headers/render_globals.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_frame_begin_parameters.h"
#include "headers/render_camera.h"
#include "headers/render_frustum.h"
#include "headers/point2d.h"
#include "headers/bitmap_data.h"
#include "headers/rasterizer_target.h"
#include "headers/blam_data_globals.h"
#include <stddef.h>   /* NULL */
#include <string.h>
#include "headers/game_time_constants.h"


#include "headers/real_rectangle2d.h"
#include "headers/render_frustum.h"
/* memcpy declared by <string.h> */
extern int game_time_get(void);
extern void rasterizer_frame_begin(const rasterizer_frame_begin_parameters *parameters);
extern uint8_t rasterizer_windows_begin(void);
extern void rasterizer_window_begin(const rasterizer_window_begin_parameters *parameters);
extern void render_camera_build_frustum(const render_camera *camera, const real_rectangle2d *frustum_bounds, render_frustum *frustum, uint8_t build_projection);
extern void render_player_frame(struct render_window *window, const point2d *screenshot_combined_index);
extern void interface_draw_fullscreen_overlays(void);
extern void rasterizer_debug_draw(void);
extern void game_engine_nonplayer_post_rasterize(void);
extern void rasterizer_window_end(void);
extern void progress_bar_eachframe(void);
extern void trouble_is_brewing_render(void);
extern void rasterizer_windows_end(void);
extern void rasterizer_frame_end(void);

void render_frame(const struct render_window *windoze, int16_t window_count, const point2d *screenshot_page_index, const point2d *screenshot_index, bitmap_data *screenshot_bitmap, float time_delta_since_tick_sec, float time_delta_since_frame_sec)
{
    render.time_delta_since_tick_sec = time_delta_since_tick_sec;
    render.time_delta_since_frame_sec = time_delta_since_frame_sec;
    ++render.frame_index;

    rasterizer_frame_begin_parameters frame_params;
    frame_params.game_time_sec = (double)game_time_get() * SECONDS_PER_TICK + time_delta_since_tick_sec;
    rasterizer_frame_begin(&frame_params);

    if (!rasterizer_windows_begin())
        return;

    for (int i = 0; i < window_count; i = (int16_t)(i + 1))
    {
        render.window_index = i;
        const struct render_window *window = &windoze[i];

        if (window->console_window)
        {
            /* Console window: fullscreen overlays + debug geometry only. */
            rasterizer_window_begin_parameters params;
            memset(&params, 0, sizeof(params));
            memcpy(&render.camera, &window->render_camera, sizeof(render.camera));
            render_camera_build_frustum(&render.camera, NULL, &render.frustum, 1u);
            memcpy(&params.camera, &window->rasterizer_camera, sizeof(params.camera));
            render_camera_build_frustum(&params.camera, NULL, &params.frustum, 1u);
            params.rasterizer_target = _rasterizer_target_render_primary;
            params.suppress_clear = 1;
            params.window_index = -1;
            rasterizer_window_begin(&params);
            interface_draw_fullscreen_overlays();
            rasterizer_debug_draw();
        }
        else if ((uint16_t)window->local_player_index != 0xFFFF)
        {
            /* Player window: full player view, optionally tagged with a screenshot tile index. */
            if (screenshot_index)
            {
                point2d combined_index;
                if (screenshot_page_index)
                {
                    combined_index.__s1.x = screenshot_page_index->__s1.x * global_screenshot_size + screenshot_index->__s1.x;
                    combined_index.__s1.y = screenshot_page_index->__s1.y * global_screenshot_size + screenshot_index->__s1.y;
                }
                render_player_frame((struct render_window *)&windoze[i], &combined_index);
            }
            else
            {
                render_player_frame((struct render_window *)&windoze[i], NULL);
            }
            continue; /* player frame manages its own window begin/end */
        }
        else
        {
            /* Other window: non-player post-rasterize pass. */
            rasterizer_window_begin_parameters params;
            memset(&params, 0, sizeof(params));
            memcpy(&render.camera, &window->render_camera, sizeof(render.camera));
            render_camera_build_frustum(&render.camera, NULL, &render.frustum, 1u);
            memcpy(&params.camera, &window->rasterizer_camera, sizeof(params.camera));
            render_camera_build_frustum(&params.camera, NULL, &params.frustum, 1u);
            params.rasterizer_target = _rasterizer_target_render_primary;
            params.suppress_clear = 0;
            params.window_index = -1;
            rasterizer_window_begin(&params);
            game_engine_nonplayer_post_rasterize();
        }

        rasterizer_window_end();
    }

    progress_bar_eachframe();
    trouble_is_brewing_render();
    rasterizer_windows_end();
    rasterizer_frame_end();
}
