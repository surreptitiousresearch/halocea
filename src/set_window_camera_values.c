/* set_window_camera_values @0x83689AD8 — populate a render window's rasterizer camera from an observer
 * result (the resolved per-player view). Copies position/forward/up, derives the vertical field of view from
 * the observer's horizontal FOV and the viewport aspect (vertical = 2*atan(tan(hfov/2) * height/width)), and
 * — for a live local player whose director perspective isn't "neutral" (3), the console is closed
 * and the game isn't paused — composites the player's camera-shake effect matrix onto the view. With no
 * observer the camera is reset to the world origin/basis with a default 80-degree horizontal FOV and 4:3
 * aspect. Finally near/far planes are taken from the rasterizer globals and, unless render is frozen, the
 * finished camera is copied into the window's render camera.
 *
 * DEVIATION: the decompiler invented a trailing `long double a3` (FPR-shadow); it is a scratch double the
 * tan() argument is staged through. The aspect division and FOV constant were rendered via __int128/magic
 * shuffles; restored to plain float math. */

#include <stdint.h>
#include "headers/render_window.h"
#include "headers/observer_result.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/rasterizer_globals.h"
#include "headers/director_perspective.h"
#include "headers/blam_data_globals.h"


extern double tan(double x);
extern double atan2(double y, double x);
extern uint8_t console_is_active(void);
extern uint8_t game_time_get_paused(void);
extern int16_t director_get_perspective(int16_t local_player_index);
extern void player_effect_get_camera_effect_matrix(int16_t local_player_index, real_matrix4x3 *matrix);
extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);
extern void matrix4x3_to_point_and_vectors(const real_matrix4x3 *matrix, real_point3d *point, real_vector3d *forward, real_vector3d *up);
extern void *memcpy(void *dst, const void *src, unsigned int count);

void set_window_camera_values(struct render_window *current_window, const observer_result *observer)
{
    render_camera *camera = &current_window->rasterizer_camera;

    if (observer)
    {
        float height = (__int16)(camera->viewport_bounds.n[2] - camera->viewport_bounds.n[0]);
        float width = (__int16)(camera->viewport_bounds.n[3] - camera->viewport_bounds.n[1]);
        float aspect = height / width;

        camera->position = observer->position;
        camera->forward = observer->forward;
        camera->up = observer->up;

        float half_horizontal_fov = (observer->field_of_view * 0.5f);
        camera->vertical_field_of_view = (float)atan2(tan(half_horizontal_fov) * aspect, 1.0) * 2.0f;

        if ((unsigned __int16)current_window->local_player_index != 0xFFFF
            && !console_is_active()
            && !game_time_get_paused()
            && director_get_perspective(current_window->local_player_index) != _director_perspective_neutral)
        {
            real_matrix4x3 effect_matrix;
            real_matrix4x3 view_matrix;
            player_effect_get_camera_effect_matrix(current_window->local_player_index, &effect_matrix);
            matrix4x3_from_point_and_vectors(&view_matrix, &observer->position, &observer->forward, &observer->up);
            matrix4x3_multiply(&view_matrix, &effect_matrix, &view_matrix);
            matrix4x3_to_point_and_vectors(&view_matrix, &camera->position, &camera->forward, &camera->up);
        }
    }
    else
    {
        /* 0.6981316804885864 = 40 degrees (half of the default 80-degree horizontal FOV). */
        camera->position = *global_origin3d;
        camera->forward = *global_forward3d;
        camera->up = *global_up3d;
        camera->vertical_field_of_view = (float)atan2(tan(0.6981316804885864) * 0.75, 1.0) * 2.0f;
    }

    camera->mirrored = 0;
    camera->z_near = rasterizer_globals.z_near;
    camera->z_far = rasterizer_globals.z_far;

    if (!debug_render_freeze)
        memcpy(&current_window->render_camera, camera, sizeof(current_window->render_camera));
}
