/* editor_camera_update @0x837C3D04 — per-frame update for the free-fly editor camera. When a scripted
 * camera is active and the controls are not, it defers to the scripted camera. Otherwise, if scripted, it
 * snaps the editor camera onto the active render camera (position + facing), refreshes its unit focus and
 * runs the camera-mode translate hook; then it always runs the camera-mode update hook, and for scripted
 * cameras forces the cut/no-blend flags and zeroes the blend timer on the result. */

#include "headers/flying_camera.h"
#include "headers/camera_control.h"
#include "headers/observer_command.h"
#include "headers/real_euler_angles2d.h"
#include "headers/render_globals.h"
#include "headers/observer_command_flags.h"
#include "headers/blam_data_globals.h"

#include "headers/real_vector3d.h"
#include "headers/dead_camera.h"
extern void (*translate_funcs[][2])(flying_camera *camera);
extern void (*update_funcs[2])(flying_camera *camera, const camera_control *controls,
                               observer_command *result);

extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);
extern void editor_camera_set_unit_focus(int unit_index);
extern void scripted_camera_update(dead_camera *camera, const camera_control *controls, observer_command *result);

void editor_camera_update(flying_camera *camera, const camera_control *controls, observer_command *result)
{
    if (!is_scripted || controls->active)
    {
        if (is_scripted)
        {
            editor_camera->position = editor_custom_render->camera.position;
            euler_angles2d_from_vector3d(&editor_camera->orientation,
                                         &editor_custom_render->camera.forward);
            editor_camera_set_unit_focus(unit_focus);
            if (camera_mode)
                translate_funcs[camera_mode][1](editor_camera);
        }

        update_funcs[camera_mode](camera, controls, result);

        if (is_scripted)
        {
            /* force a hard cut: valid + force_time (no camera blend) */
            result->flags |= (1u << _observer_command_valid_bit) | (1u << _observer_command_force_time_bit);
            result->timer = 0.0f;
        }
    }
    else
    {
        scripted_camera_update(nullptr, controls, result);
    }
}
