/* intersect_planes3d @0x8381B610 — intersect two 3D planes and express their intersection line as a 2D plane
 * (line) in the projection that drops the reference plane's dominant axis. Picks the largest-magnitude axis of
 * the reference normal, eliminates that axis from `plane` using the reference plane (or takes `plane` directly
 * when it has no component on that axis), normalizes the resulting direction, and writes the 2D line into
 * result using the projection-axis mapping table. Returns 1 on a valid intersection line, 2 when the eliminated
 * plane sits on the negative side (degenerate, behind), or 0 for the degenerate/near-zero cases.
 *
 * DEVIATION: the DB prototype's trailing `long double a4` is a phantom (FPR-shadow); it is the local reciprocal
 * scratch, reconstructed as a local. __fsqrts is used as a builtin. */

#include <stdint.h>
#include "headers/real_plane3d.h"
#include "headers/real_plane2d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);

extern double __fabs(double x);
extern int _isnan(double x);
extern uint8_t valid_real_normal3d(const real_vector3d *n);

int16_t intersect_planes3d(const real_plane3d *plane, const real_plane3d *reference_plane, real_plane2d *result)
{
    float abs_y = (float)__fabs(reference_plane->n.n[1]);
    float abs_z = (float)__fabs(reference_plane->n.n[2]);
    float abs_x = (float)__fabs(reference_plane->n.n[0]);

    int16_t dominant_axis;
    if ( abs_z < abs_y || abs_z < abs_x )
        dominant_axis = abs_y >= abs_x;
    else
        dominant_axis = 2;

    int axis = dominant_axis;

    real_vector3d direction;
    float dir_x, dir_y, dir_z, dir_d;
    if ( plane->n.n[axis] == 0.0f )
    {
        dir_x = plane->n.n[0];
        dir_y = plane->n.n[1];
        dir_z = plane->n.n[2];
        dir_d = plane->d;
        direction.n[0] = plane->n.n[0];
        direction.n[1] = plane->n.n[1];
        direction.n[2] = plane->n.n[2];
    }
    else
    {
        float ratio = plane->n.n[axis] / reference_plane->n.n[axis];
        dir_d = -((reference_plane->d * ratio) - plane->d);
        dir_x = -((reference_plane->n.n[0] * ratio) - plane->n.n[0]);
        dir_y = -((reference_plane->n.n[1] * ratio) - plane->n.n[1]);
        dir_z = -((reference_plane->n.n[2] * ratio) - plane->n.n[2]);
    }

    float magnitude = __fsqrts(((dir_x * dir_x) + ((dir_z * dir_z) + (dir_y * dir_y))));
    double reciprocal = ((float)1.0 / magnitude);

    if ( _isnan(reciprocal)
        || (direction.n[0] = ((float)1.0 / magnitude) * dir_x,
            direction.n[1] = ((float)1.0 / magnitude) * dir_y,
            direction.n[2] = ((float)1.0 / magnitude) * dir_z,
            !valid_real_normal3d(&direction))
        || magnitude == 0.0f )
    {
        if ( dir_d <= 0.0f )
            return 2;
        return 0;
    }
    else
    {
        int axis_sign = (reference_plane->n.n[axis] > 0.0f);
        int16_t axis0 = global_projection3d_mappings[axis][axis_sign][0];
        int16_t axis1 = global_projection3d_mappings[axis][axis_sign][1];
        result->d = ((float)1.0 / magnitude) * dir_d;
        float component1 = direction.n[axis1];
        result->n.n[0] = direction.n[axis0];
        result->n.n[1] = component1;
        return 1;
    }
}
