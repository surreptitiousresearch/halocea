/* main_pregame_render @ 0x836899A0 — render a frame while no game is running (menus/
 * loading): render sound, set up a default full-screen window with a forward-facing
 * camera and a vertical FOV derived from a fixed ~40deg horizontal FOV and the window
 * aspect, then submit the pregame frame.
 *
 * RESOLVED: the auto-named global block starting at 0x8441DF22 is simply the canonical
 * `window` global (struct render_window @0x8441DF20): +2 = console_window, the camera
 * fields at +0x58 are window.rasterizer_camera, and the shipped code memcpys the built
 * rasterizer_camera (84 bytes) into window.render_camera (+4) before submitting. The
 * prior `pregame_window` reconstruction modeled this block as a separate invented
 * struct; retired. */
#include "headers/main_globals.h"
#include "headers/render_window.h"

#include <math.h>
#include <string.h>
#include "headers/blam_data_globals.h"

#include "headers/rectangle2d.h"
extern void sound_render(void);
extern void compute_window_bounds(int player_index, int num_players, rectangle2d *pixel_bounds, rectangle2d *safe_frame_bounds);
extern void render_frame_pregame(const struct render_window *window, bitmap_data *screenshot_bitmap);

void main_pregame_render(void)
{
    const double horizontal_fov = 0.6981317;  /* ~40 degrees, fixed */
    float aspect_tan;

    sound_render();

    window.console_window = 1;
    window.rasterizer_camera.position.n[0] = 0.0f;
    window.rasterizer_camera.position.n[1] = 0.0f;
    window.rasterizer_camera.position.n[2] = 0.0f;
    window.rasterizer_camera.forward.n[0] = 0.0f;
    window.rasterizer_camera.forward.n[1] = 0.0f;
    window.rasterizer_camera.forward.n[2] = 1.0f;
    window.rasterizer_camera.up.n[0] = 0.0f;
    window.rasterizer_camera.up.n[1] = 1.0f;
    window.rasterizer_camera.up.n[2] = 0.0f;
    window.rasterizer_camera.mirrored = 0;
    window.local_player_index = -1;

    /* vertical FOV = 2 * atan( tan(hfov) * 0.75 / aspect ) */
    aspect_tan = (float)(tan(horizontal_fov) * 0.75);
    window.rasterizer_camera.vertical_field_of_view = (float)(atan2((double)aspect_tan, 1.0) * 2.0);

    compute_window_bounds(0, 1, &window.rasterizer_camera.viewport_bounds,
                          &window.rasterizer_camera.window_bounds);
    window.rasterizer_camera.z_far  = 1.0f;
    window.rasterizer_camera.z_near = 0.0099999998f;

    /* shipped code copies the built rasterizer camera into the render camera slot */
    memcpy(&window.render_camera, &window.rasterizer_camera, sizeof(render_camera)); /* 0x54 */

    render_frame_pregame(&window, main_globals.movie);
}
