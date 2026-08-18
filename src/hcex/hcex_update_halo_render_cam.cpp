/* hcex_update_halo_render_cam @0x8369E370 — hcex bridge: refresh one Blam render window (+1 skew: slot 0
 * is the console window, cam_idx == -1) from the observer camera of the local player driving it. Resets
 * the window's FOV to the 80° default, computes the split-screen viewport/window bounds (a single
 * full-screen window unless multiple local players share the screen and no coop override is active), then
 * overrides the FOV from the camera and pushes the camera values into the window. */

#include <stdint.h>
#include "../headers/render_window.h"
#include "../headers/observer_result.h"

#include "headers/rectangle2d.h"
extern "C" render_window hcex_window[];
extern "C" float hcex_fov[];
extern "C" int hcex_coop_local_player_index;

extern "C" int hcex_render_player_index(int cam_idx);
extern "C" const observer_result *observer_get_camera(int16_t local_player_index);
extern "C" int16_t local_player_count(void);
extern "C" void compute_window_bounds(int player_index, int num_players, rectangle2d *pixel_bounds, rectangle2d *safe_frame_bounds);
extern "C" void set_window_camera_values(struct render_window *current_window, const observer_result *observer);

extern "C" void hcex_update_halo_render_cam(int cam_idx)
{
    int window_player_index = 0;
    int window_player_count = 1;
    render_window *window = &hcex_window[cam_idx + 1];

    int16_t local_player_index = hcex_render_player_index(cam_idx);
    window->local_player_index = local_player_index;
    const observer_result *camera = observer_get_camera(local_player_index);

    hcex_fov[cam_idx + 1] = 80.0f;
    if ( cam_idx == -1 )
    {
        window->console_window = 1;
    }
    else
    {
        window->console_window = 0;
        if ( hcex_coop_local_player_index < 0 )
        {
            window_player_index = cam_idx;
            window_player_count = local_player_count();
        }
    }

    compute_window_bounds(window_player_index, window_player_count,
                          &window->rasterizer_camera.viewport_bounds,
                          &window->rasterizer_camera.window_bounds);
    if ( camera )
        hcex_fov[cam_idx + 1] = camera->field_of_view;
    set_window_camera_values(window, camera);
}
