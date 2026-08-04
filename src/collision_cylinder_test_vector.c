/* collision_cylinder_test_vector @0x83805690 — cast the segment (point → point+vector) against a finite cylinder
 * feature (axis base→base+height, radius `width`). Solves the ray/infinite-cylinder quadratic, clamps the entry
 * fraction to the span where the hit lies between the two end caps, and on success writes the radial contact
 * normal/plane. Returns TRUE on a hit. Faithful transcription; the fsel near-root clamp uses f31 == 0.0. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_plane3d.h"
#include "headers/collision_cylinder.h"

extern float __fsqrts(float);
extern float normalize3d(real_vector3d *v);

uint8_t collision_cylinder_test_vector(const collision_cylinder *cylinder, const real_point3d *point,
                                   const real_vector3d *vector, float *t, real_plane3d *plane)
{
    float hv = ((cylinder->height.n[2] * vector->n[2])
                     + ((cylinder->height.n[1] * vector->n[1]) + (cylinder->height.n[0] * vector->n[0])));
    float hh = ((cylinder->height.n[2] * cylinder->height.n[2])
                     + ((cylinder->height.n[0] * cylinder->height.n[0])
                             + (cylinder->height.n[1] * cylinder->height.n[1])));
    float vv = ((vector->n[2] * vector->n[2])
                     + ((vector->n[0] * vector->n[0]) + (vector->n[1] * vector->n[1])));
    float denom = ((vv * hh) - (hv * hv));
    if ( denom == 0.0 )
        return 0;

    float wx = point->n[0] - cylinder->base.n[0];
    float wy = point->n[1] - cylinder->base.n[1];
    float wz = point->n[2] - cylinder->base.n[2];
    float hw = ((cylinder->height.n[0] * wx)
                     + ((cylinder->height.n[2] * wz) + (cylinder->height.n[1] * wy)));
    float vw = ((vector->n[2] * wz) + ((vector->n[1] * wy) + (vector->n[0] * wx)));

    float linear_term = ((hw * hv) - (vw * hh));
    float w_squared = ((wy * wy) + ((wz * wz) + (wx * wx)));
    float bracket = ((-((cylinder->width * cylinder->width) - w_squared) * hh)
                          - (hw * hw));
    float discriminant = ((linear_term * linear_term) - (bracket * denom));
    if ( discriminant < 0.0 )
        return 0;

    float s = __fsqrts(discriminant);
    float far_root = ((s + linear_term) * ((float)1.0 / denom));
    float near_root = ((linear_term - s) * ((float)1.0 / denom));
    if ( near_root > 1.0 || far_root < 0.0 )
        return 0;

    near_root = (near_root >= 0.0) ? near_root : 0.0;   /* fsel clamp, f31 == 0.0 */
    if ( far_root > 1.0 )
        far_root = 1.0;

    if ( hv == 0.0 )
    {
        if ( hw < 0.0 || hw > hh )
            return 0;
    }
    else
    {
        float cap_base = (((float)1.0 / hv) * hw);          /* t at base cap (axial == 0) */
        float cap_top  = ((hh - hw) * ((float)1.0 / hv)); /* t at top cap (axial == hh) */
        if ( hv <= 0.0 )
        {
            if ( near_root < cap_top )
                near_root = cap_top;
            if ( far_root > -cap_base )
                far_root = -cap_base;
        }
        else
        {
            if ( near_root < -cap_base )
                near_root = -cap_base;
            if ( far_root > cap_top )
                far_root = cap_top;
        }
        if ( near_root > far_root )
            return 0;
    }

    *t = near_root;
    float hit_z = ((vector->n[2] * near_root) + wz);
    float hit_x = ((vector->n[0] * near_root) + wx);
    float hit_y = ((vector->n[1] * near_root) + wy);
    float axial = -(((cylinder->height.n[1] * hit_y)
                                 + ((cylinder->height.n[0] * hit_x) + (cylinder->height.n[2] * hit_z)))
                         / hh);
    plane->n.n[0] = (cylinder->height.n[0] * axial) + hit_x;
    plane->n.n[1] = (cylinder->height.n[1] * axial) + hit_y;
    plane->n.n[2] = (cylinder->height.n[2] * axial) + hit_z;
    if ( normalize3d(&plane->n) == 0.0 )
    {
        plane->n.n[0] = 1.0;
        plane->n.n[1] = 0.0;
        plane->n.n[2] = 0.0;
    }
    plane->d = ((cylinder->base.n[1] * plane->n.n[1])
                            + ((cylinder->base.n[0] * plane->n.n[0])
                                    + (plane->n.n[2] * cylinder->base.n[2])))
                    + cylinder->width;
    return 1;
}
