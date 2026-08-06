/* global_real_argb_grey @ 0x84176D60 (.data, 4 bytes)  DB decl: const real_argb_color *global_real_argb_grey;
 * Image bytes (big-endian), reconstructed from the binary .data record:
 *   +0x0000 = 0x82113F18 -> &private_real_argb_colors[1]   (0x82113F08 + 16*1)
 *
 * That element decodes to { alpha 1.0, red 0.5, green 0.5, blue 0.5 } = grey.
 * There is no private 16-byte constant behind this pointer: all 17 global_real_argb_<name> pointers
 * address elements of the single 272-byte private_real_argb_colors array in .rdata.
 */
#include "../headers/real_argb_color.h"

extern const real_argb_color private_real_argb_colors[17]; /* 0x82113F08 (.rdata) */

const real_argb_color *global_real_argb_grey = &private_real_argb_colors[1];
