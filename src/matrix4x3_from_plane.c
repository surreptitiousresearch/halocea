/* matrix4x3_from_plane @0x83705F38 — build an affine frame for a plane: the x-axis (n[0]) is an arbitrary unit
 * vector in the plane (a normalized perpendicular to the normal), the z-axis (n[2]) is the plane normal, the y-axis
 * (n[1]) is normal x forward, and the origin (n[3]) is the closest point on the plane to the world origin
 * (distance * normal). scale = 1. */

#include <math.h>
#include "headers/real_matrix4x3.h"
#include "headers/real_plane3d.h"
#include "headers/real_vector3d.h"

extern real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result);

void matrix4x3_from_plane(real_matrix4x3 *matrix, const real_plane3d *plane)
{
    real_vector3d forward;
    perpendicular3d(&plane->n, &forward);

    float magnitude = sqrtf((forward.n[2] * forward.n[2])
                          + ((forward.n[0] * forward.n[0]) + (forward.n[1] * forward.n[1])));
    if ( fabsf(magnitude) >= 0.0001f )
    {
        float inverse = 1.0f / magnitude;
        forward.n[0] = inverse * forward.n[0];
        forward.n[1] = inverse * forward.n[1];
        forward.n[2] = inverse * forward.n[2];
    }

    float d = plane->d;

    matrix->scale = 1.0f;
    matrix->n[0][0] = forward.n[0];
    matrix->n[0][1] = forward.n[1];
    matrix->n[0][2] = forward.n[2];

    matrix->n[1][0] = (plane->n.n[1] * forward.n[2]) - (plane->n.n[2] * forward.n[1]);
    matrix->n[1][2] = (plane->n.n[0] * forward.n[1]) - (plane->n.n[1] * forward.n[0]);
    matrix->n[1][1] = (plane->n.n[2] * forward.n[0]) - (plane->n.n[0] * forward.n[2]);

    matrix->n[2][0] = plane->n.n[0];
    matrix->n[2][1] = plane->n.n[1];
    matrix->n[2][2] = plane->n.n[2];

    matrix->n[3][0] = d * plane->n.n[0];
    matrix->n[3][1] = d * plane->n.n[1];
    matrix->n[3][2] = d * plane->n.n[2];
}
