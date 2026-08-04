/* map_to_fixed_width @0x8379C308 — linearly map `input` from [lowerBound,upperBound] to a fixed-width
 * integer in [0,range], rounding to nearest and clamping to range.
 *
 * DEVIATION: the decompiler fabricated 3 extra trailing params (a5, a6, a7) and mistyped the first three as
 * double; disasm_range(0x8379C308,0x8379C380) is pure single-precision float math (fsubs/fdivs/fmadds/frsp)
 * with only 4 real parameters, matching the DB's own prototype exactly. The tail carry-flag sequence
 * (`subfic`/`subfe`/`and`) is an identity on the already-clamped result (`x==0 ? 0 : x`, i.e. just `x`) and
 * is omitted. */

#include <math.h>

unsigned int map_to_fixed_width(float input, float lowerBound, float upperBound, unsigned int range)
{
    float t = (input - lowerBound) / (upperBound - lowerBound);
    float scaled = t * (float)range + 0.5f;
    unsigned int result = (unsigned int)(__int64)floor(scaled);

    if ( result > range )
        result = range;
    return result;
}
