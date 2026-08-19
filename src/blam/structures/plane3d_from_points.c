/* plane3d_from_points @0x837FCE78 — build a plane through three points: the (unnormalized) normal is the
 * cross product (point1-point0) x (point2-point0), stored into `plane` regardless of degeneracy. If its
 * magnitude is below 0.0001, `plane` is left with that raw (unnormalized) cross-product normal, distance
 * zeroed, and NULL is returned; otherwise the normal is normalized in place and distance is set to
 * dot(point0, normalized_normal), returning `plane`. */

#include "headers/real_plane3d.h"
#include "headers/real_point3d.h"

extern double __fabs(double x);
extern float  __fsqrts(float x);

real_plane3d *plane3d_from_points(real_plane3d *plane, const real_point3d *point0, const real_point3d *point1,
        const real_point3d *point2)
{
    float d1x = point1->n[0] - point0->n[0];
    float d1y = point1->n[1] - point0->n[1];
    float d1z = point1->n[2] - point0->n[2];
    float d2x = point2->n[0] - point0->n[0];
    float d2y = point2->n[1] - point0->n[1];
    float d2z = point2->n[2] - point0->n[2];

    plane->n.n[0] = d1y * d2z - d1z * d2y;
    plane->n.n[1] = d1z * d2x - d1x * d2z;
    plane->n.n[2] = d1x * d2y - d1y * d2x;

    float magnitude = __fsqrts(plane->n.n[0] * plane->n.n[0]
                             + plane->n.n[1] * plane->n.n[1]
                             + plane->n.n[2] * plane->n.n[2]);

    if ( __fabs(magnitude) < 0.0001f )
    {
        plane->d = 0.0f;
        return 0;
    }

    float inv_magnitude = 1.0f / magnitude;
    plane->n.n[0] *= inv_magnitude;
    plane->n.n[1] *= inv_magnitude;
    plane->n.n[2] *= inv_magnitude;
    plane->d = point0->n[0] * plane->n.n[0] + point0->n[1] * plane->n.n[1]
                     + point0->n[2] * plane->n.n[2];
    return plane;
}
