/* global_real_rgb_aqua @ 0x84176DD4 (.data, 4 bytes)  DB decl: const real_rgb_color *global_real_rgb_aqua;
 * Image bytes (big-endian), reconstructed from the binary .data record:
 *   +0x0000 = 0x82113FDC -> &private_real_argb_colors[13].rgb   (private_real_argb_colors+0x0D4 = element 13 + 4)
 *
 * That sub-object decodes big-endian to 3F000000 3F800000 3F547AE1 = { red 0.5, green 1.0, blue 0.83 } = aqua.
 * There is no private 12-byte constant behind this pointer: all 17 global_real_rgb_<name>
 * pointers address the rgb sub-object of an element of the single 272-byte
 * private_real_argb_colors array in .rdata, i.e. element base + 4, skipping the alpha channel.
 */
#include "../headers/real_argb_color.h"

extern const real_argb_color private_real_argb_colors[17]; /* 0x82113F08 (.rdata) */

const real_rgb_color *global_real_rgb_aqua = &private_real_argb_colors[13].rgb;
