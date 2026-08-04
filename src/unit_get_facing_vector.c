/* unit_get_facing_vector @0x836C9960 — return the unit's facing direction (the forward axis of its orientation;
 * object_get_orientation's up-vector output is discarded). */

#include "headers/real_vector3d.h"

extern void object_get_orientation(int object_index, real_vector3d *forward, real_vector3d *up);

void unit_get_facing_vector(int unit_index, real_vector3d *facing_vector)
{
    object_get_orientation(unit_index, facing_vector, 0);
}
