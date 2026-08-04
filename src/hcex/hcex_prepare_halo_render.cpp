/* hcex_prepare_halo_render @0x8369E6A0 — hcex bridge: stage the global render state for one Blam camera
 * (+1 skew: slot 0 = console window): reset the global window parameters onto rasterizer target 1, copy
 * the window's rasterizer camera into both the render globals and the window parameters, build the frustum
 * (with real per-player frustum bounds only when a local player drives this cam), and mirror it into the
 * window parameters. */

#include <stdint.h>
#include "../headers/render_window.h"
#include "../headers/render_globals.h"
#include "../headers/rasterizer_window_begin_parameters.h"
#include <string.h>
#include "../headers/rasterizer_target.h"

#include "headers/render_frustum.h"
extern render_globals render;
extern rasterizer_window_begin_parameters global_window_parameters;
extern __int16 global_current_render_target;
extern render_window hcex_window[];

extern __int16 hcex_render_player_index(int cam_idx);
extern void render_camera_build_frustum_bounds(const render_camera *camera, real_rectangle2d *frustum_bounds);
extern void render_camera_build_frustum(const render_camera *camera, const real_rectangle2d *frustum_bounds, render_frustum *frustum, uint8_t build_projection);

extern "C" void hcex_prepare_halo_render(int cam_idx)
{
    __int16 local_player_index = hcex_render_player_index(cam_idx);
    memset(&global_window_parameters, 0, sizeof(global_window_parameters));

    render_camera *rasterizer_camera = &hcex_window[cam_idx + 1].rasterizer_camera;
    global_current_render_target = 1;
    global_window_parameters.rasterizer_target = _rasterizer_target_render_primary;
    memcpy(&render.camera, rasterizer_camera, sizeof(render.camera));
    render.local_player_index = local_player_index;
    memcpy(&global_window_parameters.camera, rasterizer_camera, sizeof(global_window_parameters.camera));

    real_rectangle2d frustum_bounds;
    const real_rectangle2d *bounds = 0;
    if ( local_player_index != -1 )
    {
        render_camera_build_frustum_bounds(&render.camera, &frustum_bounds);
        bounds = &frustum_bounds;
    }
    render_camera_build_frustum(&render.camera, bounds, &render.frustum, 1u);
    memcpy(&global_window_parameters.frustum, &render.frustum, sizeof(global_window_parameters.frustum));
}
