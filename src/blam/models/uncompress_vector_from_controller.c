/* uncompress_vector_from_controller @0x837F7008 — DEVIATION: the decompiler rendered the controller's two
 * __int16 fields as a packed __int128 with byte-offset arithmetic; disasm_range(0x837F7008,0x837F7064)
 * shows a plain 2-arg call (matching the DB's own prototype) that just sign-extends `controller->yaw`/
 * `pitch` and scales each by 0.0031415927 (~pi/1000) into a real_euler_angles2d. */

#include "headers/real_vector3d.h"
#include "headers/direction_playback_controller.h"
#include "headers/real_euler_angles2d.h"

extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);

void uncompress_vector_from_controller(real_vector3d *vector, const direction_playback_controller *controller)
{
    real_euler_angles2d angles;

    angles.yaw = (float)controller->yaw * 0.0031415927f;
    angles.pitch = (float)controller->pitch * 0.0031415927f;
    vector3d_from_euler_angles2d(vector, &angles);
}
