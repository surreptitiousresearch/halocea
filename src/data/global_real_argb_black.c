/* global_real_argb_black @ 0x84176D64 (.data, 4 bytes)  DB decl: const real_argb_color *global_real_argb_black;
 * Image bytes (big-endian), reconstructed from the binary .data record:
 *   +0x0000 = 0x82113F28 -> &private_real_argb_colors[2]   (0x82113F08 + 16*2)
 *
 * That element decodes to { alpha 1.0, red 0.0, green 0.0, blue 0.0 } = black.
 * There is no private 16-byte constant behind this pointer: all 17 global_real_argb_<name> pointers
 * address elements of the single 272-byte private_real_argb_colors array in .rdata.
 */
#include "../headers/real_argb_color.h"

extern const real_argb_color private_real_argb_colors[17]; /* 0x82113F08 (.rdata) */

const real_argb_color *global_real_argb_black = &private_real_argb_colors[2];
