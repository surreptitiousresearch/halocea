/* fast_vector_intersects_sphere @ 0x836FDDE0 — does the segment point..point+vector touch the sphere
 * at `center` with `radius`? Solves the standard quadratic |origin + t*vector - center|^2 = radius^2
 * for t in [0,1]: a hit occurs if the origin is inside the sphere, or the ray heads toward it
 * (proj < 0) with a non-negative discriminant and the nearest crossing falls within the segment.
 *
 * Deviation: the decompiler emitted the branchless fsel cascade verbatim; it is reproduced here as
 * the equivalent nested conditionals. Intermediate names follow the quadratic: c = |d|^2 - r^2,
 * proj = vector·d, vv = |vector|^2, disc = proj^2 - vv*c, and the segment-end test tail. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

/* attested: radius is single-precision (fnmsubs f1,f1,f1,f4 @836FDE4C); return is uint8_t
 * (clrlwi r3,r11,24 @836FDE80; 11/12 callers clrlwi r3,24). */
uint8_t fast_vector_intersects_sphere(const real_point3d *point, const real_vector3d *vector,
                                      const real_point3d *center, float radius)
{
    float dx = point->n[0] - center->n[0];
    float dy = point->n[1] - center->n[1];
    float dz = point->n[2] - center->n[2];

    float proj = vector->n[2] * dz + (vector->n[0] * dx + vector->n[1] * dy);
    float vv   = vector->n[2] * vector->n[2] + (vector->n[0] * vector->n[0] + vector->n[1] * vector->n[1]);
    float c    = (dx * dx + (dz * dz + dy * dy)) - radius * radius; /* |d|^2 - r^2 */
    float disc = proj * proj - vv * c;
    float seg_end = -(vv + proj);          /* derivative of the quadratic at t = 1 */
    float tail = disc - seg_end * seg_end; /* discriminant test against the far segment end */
    int result;

    if ( c >= 0.0f )                       /* origin outside the sphere */
    {
        if ( proj >= 0.0f )                /* heading away from the sphere */
            result = 0;
        else if ( disc >= 0.0f )           /* the line crosses the sphere */
            result = (seg_end >= 0.0f) ? (tail >= 0.0f) : 1;
        else
            result = 0;
    }
    else                                   /* origin inside the sphere */
    {
        result = 1;
    }

    return result != 0;
}
