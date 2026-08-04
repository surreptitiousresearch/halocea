/* translate_orbiting_to_flying @0x837C4438 — transitions the editor camera from orbit mode to flying mode:
 * archives the current (orbiting) camera state into persisted_cameras[0], then either restores a
 * previously-saved flying camera from persisted_cameras[1] (if one was saved) or resets to a default
 * flying camera facing the active render camera's forward direction.
 *
 * DEVIATION: the decompiler's own field names ("local_player_index", "&persisted_cameras[0].saved" used as
 * a base for reading persisted_cameras[1]'s data) are artifacts of coincidentally-placed neighboring
 * symbols — disasm_range(0x837C4428,0x837C44AC) plus each symbol's own address (local_player_index sits
 * exactly 4 bytes before persisted_cameras; the "+0x20" stride to reach persisted_cameras[1].saved matches
 * sizeof(persisted_camera)) confirm the true structure: persisted_cameras is a 2-element array of
 * {flying_camera camera_data; unsigned int saved;} (see persisted_camera.h). */

#include "headers/flying_camera.h"
#include "headers/persisted_camera.h"
#include "headers/render_globals.h"
#include "headers/blam_data_globals.h"

#include "headers/real_euler_angles2d.h"
#include "headers/real_vector3d.h"
extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);

void translate_orbiting_to_flying(flying_camera *camera)
{
    persisted_cameras[0].camera_data = *camera;

    if ( persisted_cameras[1].saved )
    {
        *camera = persisted_cameras[1].camera_data;
    }
    else
    {
        camera->position.n[0] = 0.0f;
        camera->position.n[1] = 1.0f;
        camera->position.n[2] = 0.0f;
        euler_angles2d_from_vector3d(&camera->orientation, &editor_custom_render->camera.forward);
    }
}
