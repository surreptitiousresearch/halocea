/* reciprocal_square_root @0x836C5678 */
#include <math.h>

/* Param is Blam `real` (float); the decompiler defaulted it to `double` (used raw by fsqrts, no frsp).
 * DEVIATION: PPC __fsqrts (single-precision rsqrt estimate) replaced with 1/sqrtf for portability. */
float reciprocal_square_root(float x)
{
    return 1.0f / sqrtf(x);
}
