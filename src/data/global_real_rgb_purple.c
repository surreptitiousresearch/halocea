/* global_real_rgb_purple @ 0x84176DD0 (.data, 4 bytes)  DB decl: const real_rgb_color *global_real_rgb_purple;
 * Image bytes (big-endian), reconstructed from the binary .data record:
 *   +0x0000 = 0x82113FCC -> &private_real_argb_colors[12].rgb   (private_real_argb_colors+0x0C4 = element 12 + 4)
 *
 * That sub-object decodes big-endian to 3EE147AE 3D4CCCCD 3EDC28F6 = { red 0.44, green 0.05, blue 0.43 } = purple.
 * There is no private 12-byte constant behind this pointer: all 17 global_real_rgb_<name>
 * pointers address the rgb sub-object of an element of the single 272-byte
 * private_real_argb_colors array in .rdata, i.e. element base + 4, skipping the alpha channel.
 */
#include "../headers/real_argb_color.h"

extern const real_argb_color private_real_argb_colors[17]; /* 0x82113F08 (.rdata) */

const real_rgb_color *global_real_rgb_purple = &private_real_argb_colors[12].rgb;
