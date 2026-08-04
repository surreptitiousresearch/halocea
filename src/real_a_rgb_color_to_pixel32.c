/* real_a_rgb_color_to_pixel32 @0x836F7558 — pack an alpha value plus a normalized RGB color into
 * a 32-bit ARGB pixel. Each channel is scaled by 255 and truncated to a byte:
 *   alpha -> bits 31..24, red -> 23..16, green -> 15..8, blue -> 7..0.
 *
 * ABI note: alpha is passed in f1 (which reserves the r3 GPR slot), so the color pointer lands in
 * r4. The decompiler mislabelled the reserved r3 as a separate "color" parameter and named the real
 * color pointer a3; the disassembly (lfs f13/f12/f11 from r4, fmuls f7,f1) confirms the true two
 * arguments below. */

#include "headers/real_rgb_color.h"

int real_a_rgb_color_to_pixel32(float alpha, const real_rgb_color *color)
{
    unsigned int a = (unsigned char)(alpha * 255.0f);
    unsigned int r = (unsigned char)(color->n[0] * 255.0f);
    unsigned int g = (unsigned char)(color->n[1] * 255.0f);
    unsigned int b = (unsigned char)(color->n[2] * 255.0f);
    return (a << 24) | (r << 16) | (g << 8) | b;
}
