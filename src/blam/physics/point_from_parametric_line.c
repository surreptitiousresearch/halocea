/* point_from_parametric_line @0x8380BAC8 — result = p0 + forward * t.
 *
 * DEVIATION: the three stores go to `result` (the 4th parameter), not to the 5th. This file
 * previously wrote `output[0..2]` and claimed the destination was "a 5th output pointer distinct
 * from result" — the opposite of what the function does. The stores are `stfs f12, 0(r6)` /
 * `stfs f9, 4(r6)` / `stfs f6, 8(r6)`, and r6 is the 4th argument: `t` is a float, so it takes f1
 * while still CONSUMING the r5 slot, which puts `result` in r6 and leaves r7 (`output`) never
 * read. That consumed-but-unused GPR slot is the float-slot-skip hazard, and mis-reading it is
 * what shifted the destination by one parameter. Found by tools/paramdrop.py (C4100 vs prologue).
 *
 * Disasm-confirmed anomaly, unchanged: the z component reads `forward->n[2]` but adds `p0->n[1]`
 * — `lfs f7, 4(r3)` loads p0's y, not the 8(r3) that symmetry would suggest. Reproduced
 * faithfully; a latent bug in the original engine code, not a decompiler error. */

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

/* DEVIATION: previous source declared a phantom 5th param `float *output`; the binary reads only
 * r3,r4,f1,r6 (float-slot-skip: t consumes the r5 slot) — 4 args, matching the DB prototype. */
void point_from_parametric_line(real_point3d *p0, real_vector3d *forward, float t, real_point3d *result)
{
    result->n[0] = forward->n[0] * t + p0->n[0];
    result->n[1] = forward->n[1] * t + p0->n[1];
    result->n[2] = forward->n[2] * t + p0->n[1];
}
