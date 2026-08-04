/* vectors_interpolate @0x836FC950 — linear interpolation between two 3D vectors: result = a*(1-t) + b*t.
 *
 * DEVIATION: the decompiler shows a bogus 5-parameter signature with an unused `real_vector3d *result`
 * and a separate `float *a5` that's actually written; disasm_range(0x836FC950, 0x836FC950+76) confirms
 * only 4 real arguments (a, b, t, result) — r5 is `t`'s dead float-shadow GPR, and `t` itself is a plain
 * float (single-precision fmuls/fsubs throughout), not the `double` the decompiler displayed. */

#include "headers/real_vector3d.h"

void vectors_interpolate(const real_vector3d *a, const real_vector3d *b, float t, real_vector3d *result)
{
    float one_minus_t = 1.0f - t;
    result->n[0] = a->n[0] * one_minus_t + b->n[0] * t;
    result->n[1] = a->n[1] * one_minus_t + b->n[1] * t;
    result->n[2] = a->n[2] * one_minus_t + b->n[2] * t;
}
