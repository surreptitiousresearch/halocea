/* reflect_vector3d @0x836FCBF8 — reflect the incident vector about the given normal: reflection =
 * incident - 2*(normal . incident)*normal. Returns the reflection output pointer. */

#include "headers/real_vector3d.h"

real_vector3d *reflect_vector3d(const real_vector3d *incident, const real_vector3d *normal,
                                real_vector3d *reflection)
{
    float two_dot = (normal->n[0] * incident->n[0]
                     + (incident->n[1] * normal->n[1] + normal->n[2] * incident->n[2])) * 2.0f;
    reflection->n[0] = -(normal->n[0] * two_dot - incident->n[0]);
    reflection->n[1] = -(normal->n[1] * two_dot - incident->n[1]);
    reflection->n[2] = -(normal->n[2] * two_dot - incident->n[2]);
    return reflection;
}
