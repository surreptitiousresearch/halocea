/* pixel32_to_real_rgb_color @0x8377D8B8 — unpack a 32-bit ARGB pixel into a normalized RGB color.
 * Channels: red = bits 23..16, green = bits 15..8, blue = bits 7..0, each scaled by 1/255.
 *
 * Reconstructed from disassembly: the decompiler emitted "local variable allocation has failed" and
 * mislabeled the byte extractions (PPC bit numbering), so the channel mapping is taken from the
 * extrwi/clrlwi at 0x8377D8B8 and the stfs store offsets. */

#include "headers/real_rgb_color.h"

real_rgb_color *pixel32_to_real_rgb_color(unsigned int pixel, real_rgb_color *color)
{
    color->n[0] = (float)((pixel >> 16) & 0xFF) * (float)0.0039215689;   /* red   */
    color->n[1] = (float)((pixel >> 8)  & 0xFF) * (float)0.0039215689;   /* green */
    color->n[2] = (float)(pixel & 0xFF)         * (float)0.0039215689;   /* blue  */
    return color;
}
