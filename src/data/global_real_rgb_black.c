/* global_real_rgb_black @ 0x84176DA8 (.data, 4 bytes)  DB decl: const real_rgb_color *global_real_rgb_black;
 * Image bytes (big-endian), reconstructed from the binary .data record:
 *   +0x0000 = 0x82113F2C -> &private_real_argb_colors[2].rgb   (private_real_argb_colors+0x024 = element 2 + 4)
 *
 * That sub-object decodes big-endian to 00000000 00000000 00000000 = { red 0.0, green 0.0, blue 0.0 } = black.
 * There is no private 12-byte constant behind this pointer: all 17 global_real_rgb_<name>
 * pointers address the rgb sub-object of an element of the single 272-byte
 * private_real_argb_colors array in .rdata, i.e. element base + 4, skipping the alpha channel.
 */
#include "../headers/real_argb_color.h"

extern const real_argb_color private_real_argb_colors[17]; /* 0x82113F08 (.rdata) */

const real_rgb_color *global_real_rgb_black = &private_real_argb_colors[2].rgb;
