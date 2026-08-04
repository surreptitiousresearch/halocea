/* editor_camera_set_scripted @0x837C3DE8 — toggle the editor camera between free-fly and
 * scripted-camera control. Entering scripted mode (scripted != 0) suspends the current translate mode's
 * hook, snaps the scripted camera view onto the current render camera's position/facing, and starts a
 * relative scripted-camera shot (tracking the focused unit if any, otherwise the world). Leaving scripted
 * mode restores the editor camera's own position/facing, restores unit focus, and resumes the translate
 * mode's hook. Always records the previous/current scripted state and prints a status line.
 *
 * DEVIATION: the decompiler renders the scripted_camera_set_camera_point_relative call's tick_count
 * argument as an unset local and mislabels relative_to_object_index as a hardcoded 0 — both are FPR-shadow
 * artifacts (fov's float shadows a GPR slot). Disasm shows tick_count is actually the constant 0, and
 * relative_to_object_index is -1 when there's no focused unit, or unit_focus itself when there is
 * (i.e. the shot is relative to the tracked unit). */

#include <stdint.h>
#include "headers/flying_camera.h"
#include "headers/render_globals.h"
#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/blam_data_globals.h"

#include "headers/real_vector3d.h"
extern void (*translate_funcs[][2])(flying_camera *camera);

extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);
extern void editor_camera_set_position(const real_point3d *point, const real_euler_angles2d *angles);
extern void editor_camera_set_unit_focus(int unit_index);
extern void scripted_camera_set_camera_point_relative(const real_point3d *position, const real_vector3d *forward, const real_vector3d *up, float fov, int16_t tick_count, int relative_to_object_index);
extern void console_printf(uint8_t clear, const char *format, ...);

void editor_camera_set_scripted(uint8_t scripted)
{
    if ( scripted )
    {
        if ( camera_mode )
            translate_funcs[camera_mode][0](editor_camera);

        real_euler_angles2d orientation;
        euler_angles2d_from_vector3d(&orientation, &editor_custom_render->camera.forward);
        editor_camera_set_position(&editor_custom_render->camera.position, &orientation);

        if ( unit_focus == -1 )
            scripted_camera_set_camera_point_relative(&editor_custom_render->camera.position,
                &editor_custom_render->camera.forward, &editor_custom_render->camera.up, 1.2217305f,
                0, -1);
        else
            scripted_camera_set_camera_point_relative((const real_point3d *)&unit_offset,
                &editor_custom_render->camera.forward, &editor_custom_render->camera.up, 1.2217305f,
                0, unit_focus);
    }
    else
    {
        editor_camera->position = editor_custom_render->camera.position;
        euler_angles2d_from_vector3d(&editor_camera->orientation, &editor_custom_render->camera.forward);
        editor_camera_set_unit_focus(unit_focus);

        if ( camera_mode )
            translate_funcs[camera_mode][1](editor_camera);
    }

    last_scripted = is_scripted;
    is_scripted = scripted;

    console_printf(0, "%s scripted camera mode", str_1[scripted]);
}
