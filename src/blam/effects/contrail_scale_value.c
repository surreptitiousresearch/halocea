#include <stdint.h>
/* contrail_scale_value @0x8373EF08 — returns density*upper_bound when bit_index is set in scale_flags,
 * else just upper_bound (unscaled).
 *
 * DEVIATION: the decompiler renders scale_flags/bit_index as phantom trailing ints `a5`/`a6` (FPR-shadow
 * from the two float args occupying f1/f2). disasm_range (0x8373EF08-0x8373EF2C) confirms r5=scale_flags,
 * r6=bit_index — matching the DB's own 4-arg prototype exactly, no phantom params. */

float contrail_scale_value(float density, float upper_bound, unsigned int scale_flags, int16_t bit_index)
{
    if (scale_flags & (1 << bit_index))
        return density * upper_bound;
    return upper_bound;
}
