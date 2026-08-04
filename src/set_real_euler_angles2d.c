#include "headers/real_euler_angles2d.h"

/* Params are Blam `real` (float); decompiler defaulted them to `double` (stored via stfs, no frsp). */
real_euler_angles2d *set_real_euler_angles2d(real_euler_angles2d *angles, float pitch, float yaw)
{
    angles->n[1] = pitch;
    angles->n[0] = yaw;
    return angles;
}
