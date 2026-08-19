/* sphere_intersects_rectangle3d @ 0x836FF290 — true when a sphere (center,
 * radius) intersects an axis-aligned box. Distance from center to the box is
 * clamped per-axis; the sphere hits when that squared distance is below
 * radius^2. (r3=center, f1=radius, r5=bounds; r4 reserved for the float arg.) */

#include "headers/real_point3d.h"
#include "headers/real_rectangle3d.h"

int sphere_intersects_rectangle3d(const real_point3d *center, float radius,
                                  const real_rectangle3d *bounds)
{
    float dx, dy, dz;

    if (center->n[0] > bounds->n[1])
        dx = center->n[0] - bounds->n[1];
    else if (center->n[0] >= bounds->n[0])
        dx = 0.0f;
    else
        dx = bounds->n[0] - center->n[0];

    if (center->n[1] > bounds->n[3])
        dy = center->n[1] - bounds->n[3];
    else if (center->n[1] >= bounds->n[2])
        dy = 0.0f;
    else
        dy = bounds->n[2] - center->n[1];

    dz = 0.0f;
    if (center->n[2] > bounds->n[5])
        dz = center->n[2] - bounds->n[5];
    else if (center->n[2] < bounds->n[4])
        dz = bounds->n[4] - center->n[2];

    return (dx * dx + (dz * dz + dy * dy)) < (radius * radius);
}
