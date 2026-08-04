/* pixel32_to_real_argb_color @0x8377D838 — unpacks a 0xAARRGGBB pixel into a normalized ARGB color.
 *
 * DEVIATION: the decompiler flagged "local variable allocation has failed" and produced a bogus 6-param
 * signature built from __int64/HIDWORD tricks. disasm_range(0x8377D838, 0x8377D8B4) shows only r3 (pixel)
 * and r4 (color) are actually used: the four bytes of r3 are extracted (clrlwi/srwi/extrwi), individually
 * converted to float via the std/lfd/fcfid stack round-trip, scaled by the 1/255 constant @0x3b808081, and
 * stored as color->n[0..3] = {byte3, byte2, byte1, byte0} — i.e. alpha, red, green, blue in MSB-to-LSB
 * order. Reconstructed positionally from disasm; matches the established 2-arg extern used elsewhere. */

#include "headers/real_argb_color.h"

real_argb_color *pixel32_to_real_argb_color(unsigned int pixel, real_argb_color *color)
{
    const float inv255 = 0.0039215689f;

    color->n[0] = (float)((pixel >> 24) & 0xFF) * inv255; /* alpha */
    color->n[1] = (float)((pixel >> 16) & 0xFF) * inv255; /* red */
    color->n[2] = (float)((pixel >> 8) & 0xFF) * inv255;  /* green */
    color->n[3] = (float)(pixel & 0xFF) * inv255;         /* blue */
    return color;
}
