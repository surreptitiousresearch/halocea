/* translate_flying_to_orbiting @0x837C43A0 — transitions the editor camera from flying mode back to orbit
 * mode: archives the current (flying) camera state into persisted_cameras[1], then resets the camera to
 * the active render camera's position/forward direction (orbiting always re-centers rather than restoring
 * a prior orbit save).
 *
 * DEVIATION: disasm_range(0x837C43BC,0x837C4438) resolves several decompiler artifacts here: the 7-word
 * copy loop (base `camera[-1].field_of_view` == `camera - 4`, pre-incremented) is simply
 * `persisted_cameras[1].camera_data = *camera` (flying_camera is exactly 7 words); the
 * `offsetof(render_globals, frame_index)` array index folds to 0 (frame_index is render_globals' first
 * field), and the subsequent `.v`/QWORD punning is three plain word stores — together just
 * `camera->position = editor_custom_render->camera.position`. */

#include "headers/flying_camera.h"
#include "headers/persisted_camera.h"
#include "headers/render_globals.h"
#include "headers/blam_data_globals.h"

#include "headers/real_euler_angles2d.h"
#include "headers/real_vector3d.h"
extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);
extern void editor_camera_set_unit_focus(int unit_index);

void translate_flying_to_orbiting(flying_camera *camera)
{
    persisted_cameras[1].camera_data = *camera;
    persisted_cameras[1].saved = 1;

    camera->position = editor_custom_render->camera.position;
    euler_angles2d_from_vector3d(&camera->orientation, &editor_custom_render->camera.forward);
    editor_camera_set_unit_focus(unit_focus);
}
