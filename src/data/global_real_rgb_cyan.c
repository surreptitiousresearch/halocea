/* global_real_rgb_cyan @ 0x84176DB8 (.data, 4 bytes)  DB decl: const real_rgb_color *global_real_rgb_cyan;
 * Image bytes (big-endian), reconstructed from the binary .data record:
 *   +0x0000 = 0x82113F6C -> &private_real_argb_colors[6].rgb   (private_real_argb_colors+0x064 = element 6 + 4)
 *
 * That sub-object decodes big-endian to 00000000 3F800000 3F800000 = { red 0.0, green 1.0, blue 1.0 } = cyan.
 * There is no private 12-byte constant behind this pointer: all 17 global_real_rgb_<name>
 * pointers address the rgb sub-object of an element of the single 272-byte
 * private_real_argb_colors array in .rdata, i.e. element base + 4, skipping the alpha channel.
 */
#include "../headers/real_argb_color.h"

extern const real_argb_color private_real_argb_colors[17]; /* 0x82113F08 (.rdata) */

const real_rgb_color *global_real_rgb_cyan = &private_real_argb_colors[6].rgb;
