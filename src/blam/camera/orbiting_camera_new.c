/* orbiting_camera_new @ 0x837C2FF8 — initialize an orbiting camera at the given distance, with its
 * orientation derived from a facing vector. (The decompiler invents an unused 3rd param and an a4; the
 * disassembly shows the facing vector arrives in r5 and is the one fed to euler_angles2d_from_vector3d.) */

#include "headers/orbiting_camera.h"
#include "headers/real_vector3d.h"

#include "headers/real_euler_angles2d.h"
extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);

void orbiting_camera_new(orbiting_camera *camera, float distance, const real_vector3d *facing)
{
    camera->distance = distance;
    euler_angles2d_from_vector3d(&camera->orientation, facing);
}
