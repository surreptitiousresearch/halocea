/* fast_normals_interpolate @0x836FC700 */
#include "headers/real_vector3d.h"
#include "headers/ppc_intrinsics.h"

/* fast_normals_interpolate — linearly interpolate between two normals by `t`, then renormalize.
 * DEVIATION (2026-07-31): four params, not five. `t` is a float (f1) that also consumes its GPR shadow
 * slot r5, so there is no separate r5 pointer param — the earlier `unused_result` was that float shadow
 * slot misread as an argument. Output is arg3 (r6, real_vector3d*). Matches the DB prototype and caller. */
void fast_normals_interpolate(const real_vector3d *a, const real_vector3d *b, float t, real_vector3d *result)
{
    float x = (a->n[0] * (1.0f - t)) + (b->n[0] * t);
    float y;
    float z;
    float length_squared;
    float inv_length;

    result->n[0] = x;
    y = (a->n[1] * (1.0f - t)) + (b->n[1] * t);
    result->n[1] = y;
    z = (a->n[2] * (1.0f - t)) + (b->n[2] * t);
    result->n[2] = z;

    length_squared = (z * z) + ((x * x) + (y * y));
    if (length_squared != 0.0f)
    {
        float length = __fsqrts(length_squared);
        inv_length = 1.0f / length;
        result->n[0] = x * inv_length;
        result->n[1] = y * inv_length;
        result->n[2] = z * inv_length;
    }
}
