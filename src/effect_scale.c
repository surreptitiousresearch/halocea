/* effect_scale @0x836E1210 — optionally scale a value by an effect's scale_a and/or scale_b driver
 * values, gated by bit `bit_index` of two flag masks.
 *
 * DEVIATION: the decompiler emitted a phantom 6th parameter and mis-assigned the flag masks. The
 * disassembly shows exactly five registers in use: r3=effect, f1=value, r5=scale_a mask, r6=scale_b
 * mask, r7=bit_index (extsh'd). The shift amount is r7, and scale_a (0x44) is gated by r5, scale_b
 * (0x48) by r6.
 *
 * DEVIATION: bit_index is `int16_t`, not `char` — the entry `extsh r11, r7` sign-extends r7 as a
 * halfword (a `char` param would be `extsb`), proving a 16-bit source type. */

#include <stdint.h>
#include "headers/effect_datum.h"

float effect_scale(const effect_datum *effect, float value, unsigned int scale_a_flags,
        unsigned int scale_b_flags, int16_t bit_index)
{
    if ( ((1 << bit_index) & scale_a_flags) != 0 )
        value = effect->scale_a * value;
    if ( ((1 << bit_index) & scale_b_flags) != 0 )
        value = effect->scale_b * value;
    return value;
}
