/* path_attractor_weight @0x837D2B98 — weight contribution of the path input's attractor for the segment
 * [p0, p1]: find the segment point closest to the attractor; outside the attractor radius the weight is 0
 * (and *distance_reference is set to FLT_MAX); inside, *distance_reference gets the distance and the weight
 * falls off linearly, (1 - distance/radius) * attractor_weight.
 *
 * DEVIATION: the decompiler dropped the register assignments of `state` (r8) and `distance_reference` (r7),
 * leaving both as uninitialized locals; disasm confirms the accesses are state->input.attractor_point/
 * attractor_radius/attractor_weight and the out-parameter. */

#include "headers/path_state.h"

extern void closest_point_to_attractor(const real_point3d *p0, const real_point3d *p1, const real_point3d *q, real_point3d *result);
extern float __fsqrts(float x);

float path_attractor_weight(const path_state *state, const real_point3d *p0, const real_point3d *p1,
        float *distance_reference)
{
    real_point3d closest;
    closest_point_to_attractor(p0, p1, &state->input.attractor_point, &closest);

    float distance_squared =
          (closest.n[1] - state->input.attractor_point.n[1]) * (closest.n[1] - state->input.attractor_point.n[1])
        + (closest.n[0] - state->input.attractor_point.n[0]) * (closest.n[0] - state->input.attractor_point.n[0])
        + (closest.n[2] - state->input.attractor_point.n[2]) * (closest.n[2] - state->input.attractor_point.n[2]);

    if ( distance_squared >= state->input.attractor_radius * state->input.attractor_radius )
    {
        *distance_reference = 3.4028235e38f;
        return 0.0f;
    }

    float distance = __fsqrts(distance_squared);
    *distance_reference = distance;
    return (1.0f - distance / state->input.attractor_radius) * state->input.attractor_weight;
}
