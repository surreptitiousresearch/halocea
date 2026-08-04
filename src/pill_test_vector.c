/* pill_test_vector @0x837E15F8 — intersect the ray (origin + t*direction) against a "pill" (capsule): a cylinder
 * of radius `width` whose axis runs from `base` along `edge`, capped by hemispheres at each end. On a hit, writes
 * results[0] = ray fraction t and results[1] = the parameter along the edge (0 at base, 1 at the far cap).
 * Returns TRUE on a hit.
 *
 * Deviation: Hex-Rays scrambled the signature (width(f1) does not reserve a GPR; the r5 vertex-table pointer is
 * passed but unused; the two output floats were split into separate phantom args). Verified via prologue + the
 * call-site disasm: r3=base, r4=edge, f1=width, r5=unused, r6=origin, r7=direction, r8=results[2]. The cap
 * edge-parameters (0.0 at base, 1.0 at far cap) and the entering-root selection were recovered from the disasm
 * (the decompiler dropped them into bare fsel/constant registers). */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern float __fsqrts(float);
extern uint8_t sphere_test_vector(const real_point3d *center, float radius, const real_point3d *point, const real_vector3d *vector, float *t_reference);

uint8_t pill_test_vector(const real_point3d *base, const real_vector3d *edge, float width,
                                 const real_point3d *vertices /* unused */, const real_point3d *origin,
                                 const real_vector3d *direction, float *results)
{
    float wx = origin->n[0] - base->n[0];
    float wy = origin->n[1] - base->n[1];
    float wz = origin->n[2] - base->n[2];

    float ee = ((edge->n[0] * edge->n[0])
                     + ((edge->n[1] * edge->n[1]) + (edge->n[2] * edge->n[2])));
    float de = ((direction->n[0] * edge->n[0])
                     + ((direction->n[1] * edge->n[1]) + (direction->n[2] * edge->n[2])));
    float dd = ((direction->n[0] * direction->n[0])
                     + ((direction->n[1] * direction->n[1]) + (direction->n[2] * direction->n[2])));
    float denom = ((dd * ee) - (de * de));
    if ( denom == 0.0 )
        return 0;

    float we = ((edge->n[0] * wx) + ((edge->n[1] * wy) + (edge->n[2] * wz)));
    float wd = ((direction->n[0] * wx)
                     + ((direction->n[1] * wy) + (direction->n[2] * wz)));
    float ww = ((wy * wy) + ((wx * wx) + (wz * wz)));

    float linear_coefficient = ((we * de) - (wd * ee));
    float bracket = (((ww - (width * width)) * ee) - (we * we));
    float discriminant = ((linear_coefficient * linear_coefficient) - (bracket * denom));
    if ( discriminant < 0.0 )
        return 0;

    float s = __fsqrts(discriminant);
    float root1 = (-((s + linear_coefficient) * ((float)1.0 / denom)));
    if ( root1 > 1.0 )
        return 0;
    float root2 = (-((linear_coefficient - s) * ((float)1.0 / denom)));
    if ( root2 < 0.0 )
        return 0;

    float t = (root1 >= 0.0) ? root1 : root2;   /* entering root */
    float edge_projection = ((t * de) + we);
    if ( edge_projection >= 0.0 )
    {
        if ( edge_projection <= ee )            /* hit on the cylinder body */
        {
            results[0] = t;
            results[1] = (edge_projection / ee);
            return 1;
        }
        real_point3d far_cap;                   /* hemisphere at the far end */
        far_cap.n[0] = edge->n[0] + base->n[0];
        far_cap.n[1] = edge->n[1] + base->n[1];
        far_cap.n[2] = edge->n[2] + base->n[2];
        if ( sphere_test_vector(&far_cap, width, origin, direction, &results[0]) )
        {
            results[1] = 1.0;
            return 1;
        }
    }
    else if ( sphere_test_vector(base, width, origin, direction, &results[0]) )  /* hemisphere at the base */
    {
        results[1] = 0.0;
        return 1;
    }
    return 0;
}
