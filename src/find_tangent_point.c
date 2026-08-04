/* find_tangent_point @0x8381E4D8 — find the point on the circle (`center`, `radius`) at which a line from
 * `point` is tangent, picking whichever of the two tangent solutions matches `clockwise` (XORed against the
 * sign of a handedness cross-product test). If `point` is inside or on the circle (no real tangent exists),
 * falls back to `center + radius * normalize(point - center)` (or `+ radius * global_left2d` if `point`
 * coincides with `center`).
 *
 * DEVIATION — dead-float-shadow-GPR pattern, same as find_avoidance_point: the DB's own decompile/prototype
 * fabricate a 6th parameter (`float *a6`, unnamed) that receives every real write, while the declared 5th
 * parameter (`tangent_point`, correctly typed `real_point2d *`) is never referenced for writes — BUT its
 * symbolic name IS reused (misleadingly) by Hex-Rays for an unrelated boolean read of the true `clockwise`
 * register later in the body (`(unsigned __int8)tangent_point != ...`). disasm_range (0x8381E4D8-0x8381E61C)
 * and `ctree_lvars` confirm the true bindings: `radius` (param 3, float despite the decompiler's `double`) is
 * passed in f1 and consumes a dead shadow at the position between `center` and `clockwise`; `clockwise` is
 * really r6; the true output pointer is r7 (declared param 5, `tangent_point`); `a6` (param 6) is never
 * referenced by any instruction and is discarded below.
 *
 * DEVIATION — the two candidate tangent points are stack-adjacent (`ctree_lvars` stkoff 0/4/8/12), but
 * `ctree_lvars`-confirmed offsets show they are NOT the two pairs the decompiler's own cross-product test
 * expression names together (`v16`+`v25` and `back_chain`+`v15`) — the true physical pairing (by stack
 * address, verified via stkoff) is candidate0 = (v16, v15-formula) at offset 0/4, candidate1 = (back_chain,
 * v25) at offset 8/12. The selection test itself is reproduced literally (using the decompiler's own named
 * value pairing, which the original source apparently computed that way for the handedness check only) but
 * the two candidates returned are grouped by their real stack offsets, not by the test expression's pairing. */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"
#include "headers/blam_data_globals.h"

extern double __fabs(double x);
extern float __fsqrts(float x);

void find_tangent_point(const real_point2d *point, const real_point2d *center, float radius,
        uint8_t clockwise, real_point2d *tangent_point)
{
    float dx = point->n[0] - center->n[0];
    float dy = point->n[1] - center->n[1];
    float dist2 = dx * dx + dy * dy;

    if (dist2 - radius * radius <= 0.0f)
    {
        float distance = __fsqrts(dist2);
        float direction_x, direction_y;

        if (__fabs(distance) < 0.000099999997f || distance == 0.0f)
        {
            direction_x = global_left2d->n[0];
            direction_y = global_left2d->n[1];
        }
        else
        {
            direction_x = dx / distance;
            direction_y = dy / distance;
        }

        tangent_point->n[0] = direction_x * radius + center->n[0];
        tangent_point->n[1] = direction_y * radius + center->n[1];
        return;
    }

    float h = __fsqrts(dist2 - radius * radius);
    float inv_scale = radius / dist2;

    real_point2d candidate0;
    candidate0.n[0] = (dx * radius + dy * h) * inv_scale + center->n[0];
    candidate0.n[1] = (dy * radius - dx * h) * inv_scale + center->n[1];

    real_point2d candidate1;
    candidate1.n[0] = (dx * radius - dy * h) * inv_scale + center->n[0];
    candidate1.n[1] = (dx * h + dy * radius) * inv_scale + center->n[1];

    unsigned __int8 cross_positive =
            (candidate1.n[1] - point->n[1]) * (candidate0.n[0] - point->n[0])
                    - (candidate1.n[0] - point->n[0]) * (candidate0.n[1] - point->n[1])
            > 0.0f;

    *tangent_point = (clockwise != 0) != (cross_positive != 0) ? candidate1 : candidate0;
}
