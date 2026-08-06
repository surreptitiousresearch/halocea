/* global_real_rgb_lightblue @ 0x84176DC8 (.data, 4 bytes)  DB decl: const real_rgb_color *global_real_rgb_lightblue;
 * Image bytes (big-endian), reconstructed from the binary .data record:
 *   +0x0000 = 0x82113FAC -> &private_real_argb_colors[10].rgb   (private_real_argb_colors+0x0A4 = element 10 + 4)
 *
 * That sub-object decodes big-endian to 3EC7AE14 3F147AE1 3F6E147B = { red 0.39, green 0.58, blue 0.93 } = lightblue.
 * There is no private 12-byte constant behind this pointer: all 17 global_real_rgb_<name>
 * pointers address the rgb sub-object of an element of the single 272-byte
 * private_real_argb_colors array in .rdata, i.e. element base + 4, skipping the alpha channel.
 */
#include "../headers/real_argb_color.h"

extern const real_argb_color private_real_argb_colors[17]; /* 0x82113F08 (.rdata) */

const real_rgb_color *global_real_rgb_lightblue = &private_real_argb_colors[10].rgb;
