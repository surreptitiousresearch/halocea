/* point_from_parametric_line @0x8380BAC8 — result = p0 + forward * t, written to a 5th output pointer
 * distinct from `result` (matches the DB's 5-argument signature). Disasm-confirmed anomaly: the z component
 * reads `forward->n[2]` but adds `p0->n[1]` (not `p0->n[2]`) — `lfs f7, 4(r3)` loads p0's y, not z, at
 * offset 8(r3) as symmetry would suggest. Reproduced faithfully; this looks like a latent bug in the
 * original engine code, not a decompiler error. */

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

void point_from_parametric_line(real_point3d *p0, real_vector3d *forward, float t, real_point3d *result,
        float *output)
{
    output[0] = forward->n[0] * t + p0->n[0];
    output[1] = forward->n[1] * t + p0->n[1];
    output[2] = forward->n[2] * t + p0->n[1];
}
