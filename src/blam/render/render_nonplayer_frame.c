/* render_nonplayer_frame @0x837071A0 — render a non-gameplay frame into a window (UI/loading screens and
 * engine-driven overlays): seed the global render camera and a local rasterizer window-begin block from the
 * window's two cameras, build both frusta, begin the window on rasterizer target 1 (no window index,
 * clear suppressed only when nonplayer_type == 0 — the branchless cntlzw test), then draw either the game
 * engine's post-rasterize content (nonplayer_type != 0) or the fullscreen interface overlays + debug draw.
 *
 * DEVIATION: the decompiler threads the callees' r3 leftovers into rasterizer_window_end as an argument;
 * all three draw callees and rasterizer_window_end are void (see their files). */

#include <stdint.h>
#include "headers/render_window.h"
#include "headers/render_globals.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include <string.h>
#include "headers/rasterizer_target.h"
#include "headers/blam_data_globals.h"


#include "headers/real_rectangle2d.h"
#include "headers/render_frustum.h"
extern void render_camera_build_frustum(const render_camera *camera, const real_rectangle2d *frustum_bounds, render_frustum *frustum, uint8_t build_projection);
extern void rasterizer_window_begin(const rasterizer_window_begin_parameters *parameters);
extern void rasterizer_window_end(void);
extern void game_engine_nonplayer_post_rasterize(void);
extern void interface_draw_fullscreen_overlays(void);
extern void rasterizer_debug_draw(void);

void render_nonplayer_frame(struct render_window *window, unsigned int nonplayer_type)
{
    rasterizer_window_begin_parameters parameters;
    memset(&parameters, 0, sizeof(parameters));

    memcpy(&render.camera, &window->render_camera, sizeof(render.camera));
    render_camera_build_frustum(&render.camera, 0, &render.frustum, 1u);

    memcpy(&parameters.camera, &window->rasterizer_camera, sizeof(parameters.camera));
    render_camera_build_frustum(&parameters.camera, 0, &parameters.frustum, 1u);

    parameters.rasterizer_target = _rasterizer_target_render_primary;
    parameters.window_index = -1;
    parameters.suppress_clear = nonplayer_type == 0;
    rasterizer_window_begin(&parameters);

    if ( nonplayer_type )
    {
        game_engine_nonplayer_post_rasterize();
    }
    else
    {
        interface_draw_fullscreen_overlays();
        rasterizer_debug_draw();
    }
    rasterizer_window_end();
}
