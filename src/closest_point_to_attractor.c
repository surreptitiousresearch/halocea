/* closest_point_to_attractor @0x837D2AE0 — project attractor point `q` onto the infinite line through
 * segment [p0,p1], parameterized by t in [0,1]; if the projection falls outside the segment, returns p1,
 * otherwise the projected point. */

#include "headers/real_point3d.h"

void closest_point_to_attractor(const real_point3d *p0, const real_point3d *p1, const real_point3d *q, real_point3d *result)
{
    float dx = p1->n[0] - p0->n[0];
    float dy = p1->n[1] - p0->n[1];
    float dz = p1->n[2] - p0->n[2];

    float t = ((p0->n[1] - q->n[1]) * dy + ((p0->n[0] - q->n[0]) * dx + (p0->n[2] - q->n[2]) * dz))
            / (dy * dy + (dx * dx + dz * dz));

    if ( t < 0.0f || t > 1.0f )
    {
        *result = *p1;
    }
    else
    {
        result->n[0] = dx * t + p0->n[0];
        result->n[1] = dy * t + p0->n[1];
        result->n[2] = dz * t + p0->n[2];
    }
}
