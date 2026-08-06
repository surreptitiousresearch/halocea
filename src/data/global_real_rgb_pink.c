/* global_real_rgb_pink @ 0x84176DC4 (.data, 4 bytes)  DB decl: const real_rgb_color *global_real_rgb_pink;
 * Image bytes (big-endian), reconstructed from the binary .data record:
 *   +0x0000 = 0x82113F9C -> &private_real_argb_colors[9].rgb   (private_real_argb_colors+0x094 = element 9 + 4)
 *
 * That sub-object decodes big-endian to 3F800000 3ED1EB85 3F333333 = { red 1.0, green 0.41, blue 0.7 } = pink.
 * There is no private 12-byte constant behind this pointer: all 17 global_real_rgb_<name>
 * pointers address the rgb sub-object of an element of the single 272-byte
 * private_real_argb_colors array in .rdata, i.e. element base + 4, skipping the alpha channel.
 */
#include "../headers/real_argb_color.h"

extern const real_argb_color private_real_argb_colors[17]; /* 0x82113F08 (.rdata) */

const real_rgb_color *global_real_rgb_pink = &private_real_argb_colors[9].rgb;
