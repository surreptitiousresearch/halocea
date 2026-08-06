/* global_real_rgb_salmon @ 0x84176DDC (.data, 4 bytes)  DB decl: const real_rgb_color *global_real_rgb_salmon;
 * Image bytes (big-endian), reconstructed from the binary .data record:
 *   +0x0000 = 0x82113FFC -> &private_real_argb_colors[15].rgb   (private_real_argb_colors+0x0F4 = element 15 + 4)
 *
 * That sub-object decodes big-endian to 3F800000 3F2147AE 3EF5C28F = { red 1.0, green 0.63, blue 0.48 } = salmon.
 * There is no private 12-byte constant behind this pointer: all 17 global_real_rgb_<name>
 * pointers address the rgb sub-object of an element of the single 272-byte
 * private_real_argb_colors array in .rdata, i.e. element base + 4, skipping the alpha channel.
 */
#include "../headers/real_argb_color.h"

extern const real_argb_color private_real_argb_colors[17]; /* 0x82113F08 (.rdata) */

const real_rgb_color *global_real_rgb_salmon = &private_real_argb_colors[15].rgb;
