/* rotate_vector2d @0x837AC2D0 — rotates v by (sine, cosine) into result, returns result.
 *
 * DEVIATION: decompiler renders trailing phantom `int a5, float *a6` params and writes through `a6` instead
 * of `result`. disasm_range (0x837AC2D0-0x837AC2F4) shows only r3 (v), f1 (sine), f2 (cosine), r6 (result)
 * are read — sine/cosine's FPR arguments shadow r4/r5 (never touched), pushing the real `result` pointer to
 * r6; matches the DB's own 4-arg prototype exactly. */

#include "headers/real_vector2d.h"

real_vector2d * rotate_vector2d(const real_vector2d *v, float sine, float cosine, real_vector2d *result)
{
    result->n[1] = v->n[0] * sine + v->n[1] * cosine;
    result->n[0] = v->n[0] * cosine - v->n[1] * sine;
    return result;
}
