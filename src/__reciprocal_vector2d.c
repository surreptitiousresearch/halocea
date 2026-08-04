/* __reciprocal_vector2d @0x83685A50 — component-wise reciprocal of a 2d vector: returns
 * { 1/v.i, 1/v.j } as a real_vector2d by value (both floats packed in r3).
 *
 * DEVIATION: Hex-Rays mis-modelled this as a single-component store through the return
 * pointer. Disasm is unambiguous: lfs f13,0(r3); lfs f12,4(r3); fdivs by 1.0f into both
 * slots; ld r3 returns the 8-byte struct by value. Reconstructed accordingly. */

#include "headers/real_vector2d.h"

real_vector2d __reciprocal_vector2d(const real_vector2d *v)
{
    real_vector2d result;
    result.i = 1.0f / v->i;
    result.j = 1.0f / v->j;
    return result;
}
