/* pill_test_vector3d @0x836FDBE8 — sweep the ray (point, vector) against a capsule ("pill"): a cylinder of
 * radius `width` running from `base` up its local +Z by `height`, capped by a sphere at each end. Reports the
 * first hit as a fraction along the ray (*t_reference) and the surface normal at that hit (*normal). The XY
 * distance of `point` from the axis decides the case: inside the radius the entry is immediate; outside, a 2D
 * quadratic finds the cylinder-wall crossing, and the axial coordinate of that crossing selects the cylinder
 * body (write a radial normal) or one of the two spherical caps (delegate to sphere_test_vector3d). A hit whose
 * normal faces along the ray (dot > 0) is a back face and is rejected. Returns 1 on a valid hit, else 0.
 *
 * DEVIATION: the DB prototype (honored here) is 7 args; on Xbox360 PPC the two float args (height, width)
 * each burn a shadow GPR slot, which Hex-Rays surfaced as phantom pointer params `point`/`vector` and pushed
 * the real r6/r7/r8/r9 args into t_reference/normal/a8/a9. The register→param mapping (r6=point, r7=vector,
 * r8=t_reference out, r9=normal out) and every sphere_test_vector3d call were recovered from the disassembly;
 * register-pun doubles are plain float math. The axial coordinate is divided by height*height as shipped. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern float __fsqrts(float);
extern uint8_t sphere_test_vector3d(const real_point3d *center, float radius, const real_point3d *origin, const real_vector3d *direction, float *out_t, real_vector3d *out_normal);

uint8_t pill_test_vector3d(const real_point3d *base, float height, float width,
        const real_point3d *point, const real_vector3d *vector, float *t_reference, real_vector3d *normal)
{
    float dx = point->n[0] - base->n[0];
    float dy = point->n[1] - base->n[1];
    float axial_dot = vector->n[0] * dx + dy * vector->n[1];         /* (point-base)_xy . vector_xy */
    float radial = (dx * dx + dy * dy) - width * width;               /* |point-base|²_xy - radius² */

    float t;
    if ( radial > 0.0f )
    {
        float xy_len_sq = vector->n[1] * vector->n[1] + vector->n[0] * vector->n[0];
        float discriminant = axial_dot * axial_dot - xy_len_sq * radial;
        if ( discriminant < 0.0f )
            return 0;
        float root = __fsqrts(discriminant) + axial_dot;
        if ( -root > xy_len_sq )
            return 0;
        t = -root / xy_len_sq;
    }
    else
    {
        t = 0.0f;
    }

    uint8_t result;
    float axial_coord = (t * vector->n[2] + (point->n[2] - base->n[2])) / (height * height);
    if ( axial_coord < 0.0f )
    {
        result = sphere_test_vector3d(base, width, point, vector, t_reference, normal);
    }
    else if ( axial_coord > 1.0f )
    {
        real_point3d top_center;
        top_center.n[0] = base->n[0];
        top_center.n[1] = base->n[1];
        top_center.n[2] = base->n[2] + height;
        result = sphere_test_vector3d(&top_center, width, point, vector, t_reference, normal);
    }
    else if ( axial_dot >= 0.0f )
    {
        return 0;   /* ray receding from the axis — no entry hit on the cylinder wall */
    }
    else
    {
        *t_reference = t;
        float nx = vector->n[0] * t + dx;
        float ny = t * vector->n[1] + dy;
        normal->n[0] = nx;
        normal->n[1] = ny;
        float len_sq = nx * nx + ny * ny;
        if ( len_sq != 0.0f )
        {
            float inverse_length = 1.0f / __fsqrts(len_sq);
            normal->n[0] = nx * inverse_length;
            normal->n[1] = ny * inverse_length;
        }
        normal->n[2] = 0.0f;
        result = 1;
    }

    if ( result
      && (vector->n[0] * normal->n[0]
        + (normal->n[2] * vector->n[2] + normal->n[1] * vector->n[1])) > 0.0f )
    {
        return 0;
    }
    return result;
}
