/* sphere_intersects_cone3d @0x836FF750 — test whether a sphere (center, radius) overlaps a cone defined by
 * apex `base`, unit axis `direction`, length `distance`, and half-angle whose sine/cosine are supplied.
 * First rejects on the axial slab (projection outside [-radius, radius+distance]), then applies the
 * standard sphere-vs-cone angular test. Returns 1 on intersection, 0 otherwise.
 *
 * The decompiler mis-rendered the signature through the FPR-shadow ABI (the float radius/distance/sine/
 * cosine each reserve a GPR slot): it labelled the apex point `direction` and the cone axis `a8`. The real
 * argument order is (center, radius, base, direction, distance, sine, cosine) per the DB prototype. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

uint8_t sphere_intersects_cone3d(const real_point3d *center, float radius, const real_point3d *base, const real_vector3d *direction, float distance, float sine, float cosine)
{
    float offset_x = center->n[0] - base->n[0];
    float offset_y = center->n[1] - base->n[1];
    float offset_z = center->n[2] - base->n[2];

    float projection = direction->n[0] * offset_x + (direction->n[2] * offset_z + direction->n[1] * offset_y);
    if (projection < -radius)
        return 0;
    if (projection > radius + distance)
        return 0;

    float offset_length_squared = offset_y * offset_y + (offset_x * offset_x + offset_z * offset_z);
    if ((radius * sine * 2.0f + projection) * projection + radius * radius
        < offset_length_squared * cosine * cosine)
        return 0;

    return 1;
}
