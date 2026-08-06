/* global_real_argb_purple @ 0x84176D8C (.data, 4 bytes)  DB decl: const real_argb_color *global_real_argb_purple;
 * Image bytes (big-endian), reconstructed from the binary .data record:
 *   +0x0000 = 0x82113FC8 -> &private_real_argb_colors[12]   (0x82113F08 + 16*12)
 *
 * That element decodes to { alpha 1.0, red 0.44, green 0.05, blue 0.43 } = purple.
 * There is no private 16-byte constant behind this pointer: all 17 global_real_argb_<name> pointers
 * address elements of the single 272-byte private_real_argb_colors array in .rdata.
 */
#include "../headers/real_argb_color.h"

extern const real_argb_color private_real_argb_colors[17]; /* 0x82113F08 (.rdata) */

const real_argb_color *global_real_argb_purple = &private_real_argb_colors[12];
