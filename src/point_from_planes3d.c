/* point_from_planes3d @0x836FFF38 — find the point where three planes intersect, via the standard
 * three-plane Cramer's-rule solution: point = (d0*(n1xn2) + d1*(n2xn0) + d2*(n0xn1)) / (n0.(n1xn2)). Fails
 * (returns 0, `point` untouched) if the planes are near-parallel (determinant below epsilon).
 *
 * DEVIATION: the decompiler fully inlines every cross/dot product into deeply nested per-component
 * expressions, writing each of `point`'s components multiple times as partial sums accumulate before the
 * final overwrite. Algebraically factored into three named cross products (verified term-by-term against
 * each field's FINAL assignment, not the intermediate ones) — no change in arithmetic performed. */

#include <stdint.h>
#include "headers/real_plane3d.h"
#include "headers/real_point3d.h"

extern double __fabs(double x);

/* attested: return is uint8_t (li r3,1 @836FFFB4 / li r3,0 @837000AC; caller
 * collision_move_point re-normalizes with clrlwi r3,24 @837747D0). */
uint8_t point_from_planes3d(const real_plane3d *plane0, const real_plane3d *plane1, const real_plane3d *plane2,
    real_point3d *point)
{
    real_vector3d cross_n0_n1, cross_n1_n2, cross_n2_n0;

    cross_n0_n1.n[0] = plane0->n.n[1] * plane1->n.n[2] - plane0->n.n[2] * plane1->n.n[1];
    cross_n0_n1.n[1] = plane0->n.n[2] * plane1->n.n[0] - plane0->n.n[0] * plane1->n.n[2];
    cross_n0_n1.n[2] = plane0->n.n[0] * plane1->n.n[1] - plane0->n.n[1] * plane1->n.n[0];

    float det = plane2->n.n[0] * cross_n0_n1.n[0] + plane2->n.n[1] * cross_n0_n1.n[1]
              + plane2->n.n[2] * cross_n0_n1.n[2];

    if ( __fabs(det) < 0.0001f )
        return 0;

    float inv_det = 1.0f / det;

    cross_n1_n2.n[0] = plane1->n.n[1] * plane2->n.n[2] - plane1->n.n[2] * plane2->n.n[1];
    cross_n1_n2.n[1] = plane1->n.n[2] * plane2->n.n[0] - plane1->n.n[0] * plane2->n.n[2];
    cross_n1_n2.n[2] = plane1->n.n[0] * plane2->n.n[1] - plane1->n.n[1] * plane2->n.n[0];

    cross_n2_n0.n[0] = plane2->n.n[1] * plane0->n.n[2] - plane2->n.n[2] * plane0->n.n[1];
    cross_n2_n0.n[1] = plane2->n.n[2] * plane0->n.n[0] - plane2->n.n[0] * plane0->n.n[2];
    cross_n2_n0.n[2] = plane2->n.n[0] * plane0->n.n[1] - plane2->n.n[1] * plane0->n.n[0];

    for ( int i = 0; i < 3; ++i )
        point->n[i] = (plane0->d * cross_n1_n2.n[i] + plane1->d * cross_n2_n0.n[i]
                     + plane2->d * cross_n0_n1.n[i]) * inv_det;

    return 1;
}
