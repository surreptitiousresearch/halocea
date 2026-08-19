/* rgb_color_to_real_rgb_color @0x8377D7C8 — convert a 16-bit-per-channel rgb_color to a normalized
 * (0..1) real_rgb_color, scaling by 1/65535.
 *
 * DEVIATION: the decompiler fabricated a 4-arg __int64-juggling signature (real_rgb, a3, a4); the DB's own
 * prototype and disasm_range(0x8377D7C8,0x8377D838) confirm a plain 2-arg function — r4 is the output
 * real_rgb_color* pointer (also the return value, `mr r3, r4` at the end). */

#include "headers/rgb_color.h"
#include "headers/real_rgb_color.h"

real_rgb_color * rgb_color_to_real_rgb_color(const rgb_color *rgb, real_rgb_color *real_rgb)
{
    real_rgb->red   = (float)rgb->__s1.red   * 0.000015259022f;
    real_rgb->green = (float)rgb->__s1.green * 0.000015259022f;
    real_rgb->blue  = (float)rgb->__s1.blue  * 0.000015259022f;
    return real_rgb;
}
