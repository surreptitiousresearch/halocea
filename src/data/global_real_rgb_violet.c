/* global_real_rgb_violet @ 0x84176DE0 (.data, 4 bytes)  DB decl: const real_rgb_color *global_real_rgb_violet;
 * Image bytes (big-endian), reconstructed from the binary .data record:
 *   +0x0000 = 0x8211400C -> &private_real_argb_colors[16].rgb   (private_real_argb_colors+0x104 = element 16 + 4)
 *
 * That sub-object decodes big-endian to 3F4F5C29 3E051EB8 3F0F5C29 = { red 0.81, green 0.13, blue 0.56 } = violet.
 * There is no private 12-byte constant behind this pointer: all 17 global_real_rgb_<name>
 * pointers address the rgb sub-object of an element of the single 272-byte
 * private_real_argb_colors array in .rdata, i.e. element base + 4, skipping the alpha channel.
 */
#include "../headers/real_argb_color.h"

extern const real_argb_color private_real_argb_colors[17]; /* 0x82113F08 (.rdata) */

const real_rgb_color *global_real_rgb_violet = &private_real_argb_colors[16].rgb;
