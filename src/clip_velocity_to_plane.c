/* clip_velocity_to_plane @0x83774378 — projects velocity onto plane's surface: subtracts the
 * component of velocity along the plane normal, leaving only the tangential component. */

#include "headers/real_vector3d.h"
#include "headers/real_plane3d.h"

void clip_velocity_to_plane(const real_vector3d *velocity, const real_plane3d *plane, real_vector3d *result)
{
    float normal_component = plane->n.n[0] * velocity->n[0]
                            + plane->n.n[1] * velocity->n[1]
                            + plane->n.n[2] * velocity->n[2];

    result->n[0] = velocity->n[0] - normal_component * plane->n.n[0];
    result->n[1] = velocity->n[1] - normal_component * plane->n.n[1];
    result->n[2] = velocity->n[2] - normal_component * plane->n.n[2];
}
