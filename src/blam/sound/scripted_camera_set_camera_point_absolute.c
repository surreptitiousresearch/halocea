/* scripted_camera_set_camera_point_absolute @0x83705118 — tail-call forwarder (`li r8, -1;
 * b scripted_camera_set_camera_point_relative`; confirmed via disasm, not a bare blr). The "absolute"
 * variant is just the "relative" one with relative_to_object_index forced to -1 (no reference object).
 * scripted_camera_set_camera_point_relative itself is not yet decompiled — declared extern with its own
 * DB prototype. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern void scripted_camera_set_camera_point_relative(const real_point3d *position, const real_vector3d *forward, const real_vector3d *up, float fov, int16_t tick_count, int relative_to_object_index);

void scripted_camera_set_camera_point_absolute(const real_point3d *position, const real_vector3d *forward, const real_vector3d *up, float fov, int16_t tick_count)
{
    scripted_camera_set_camera_point_relative(position, forward, up, fov, tick_count, -1);
}
