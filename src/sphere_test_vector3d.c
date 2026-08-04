/* sphere_test_vector3d @ 0x836FDAA0 — intersect a ray (origin + t*direction, t in [0,1]) with a
 * sphere, returning the entry hit. Returns 0 if the ray points away from the sphere, misses it, or
 * the entry lies beyond the segment. On a hit it writes the entry fraction to *out_t and the
 * outward surface normal at the hit to *out_normal; if the origin is already inside the sphere the
 * fraction is 0 and the normal is the direction from the centre to the origin.
 *
 * Deviation: the database parameter names are misleading (it mislabels origin/direction/out-t/
 * out-normal). The names here follow the actual body. Param count is 6: the float `radius` is
 * passed in f1 and shadows the r4 GPR slot (soft-float ABI) — there is NO separate unused
 * argument (an earlier reconstruction mistook the f1 shadow slot for a dead void* param). */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern double __fsqrts(double x);
extern void fast_normalize3d(real_vector3d *v);

uint8_t sphere_test_vector3d(const real_point3d *center, float radius,
                             const real_point3d *origin, const real_vector3d *direction,
                             float *out_t, real_vector3d *out_normal)
{
    float dx = origin->n[0] - center->n[0];
    float dy = origin->n[1] - center->n[1];
    float dz = origin->n[2] - center->n[2];

    float proj = direction->n[0] * dx + (direction->n[2] * dz + direction->n[1] * dy);
    float d_squared = dy * dy + (dz * dz + dx * dx);
    float c = -((radius * radius) - d_squared); /* |d|^2 - r^2 */
    float dir_squared;
    float disc;
    float root;
    float t;

    if ( proj >= 0.0f )            /* heading away from the sphere */
        return 0;

    if ( c <= 0.0f )               /* origin inside (or on) the sphere */
    {
        float len = (float)__fsqrts(d_squared);
        float inv = 1.0f / len;
        *out_t = 0.0f;
        out_normal->n[0] = dx * inv;
        out_normal->n[1] = inv * dy;
        out_normal->n[2] = dz * inv;
        return 1;
    }

    dir_squared = direction->n[0] * direction->n[0]
                + (direction->n[1] * direction->n[1] + direction->n[2] * direction->n[2]);
    disc = proj * proj - dir_squared * c;
    if ( disc < 0.0f )
        return 0;

    root = (float)__fsqrts(disc) + proj;
    if ( -root > dir_squared )     /* entry past the segment end */
        return 0;

    t = -root / dir_squared;
    *out_t = t;
    out_normal->n[0] = direction->n[0] * t + dx;
    out_normal->n[1] = direction->n[1] * t + dy;
    out_normal->n[2] = direction->n[2] * t + dz;
    fast_normalize3d(out_normal);
    return 1;
}
