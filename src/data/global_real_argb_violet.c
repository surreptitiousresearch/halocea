/* global_real_argb_violet @ 0x84176D9C (.data, 4 bytes)  DB decl: const real_argb_color *global_real_argb_violet;
 * Image bytes (big-endian), reconstructed from the binary .data record:
 *   +0x0000 = 0x82114008 -> &private_real_argb_colors[16]   (0x82113F08 + 16*16)
 *
 * That element decodes to { alpha 1.0, red 0.81, green 0.13, blue 0.56 } = violet.
 * There is no private 16-byte constant behind this pointer: all 17 global_real_argb_<name> pointers
 * address elements of the single 272-byte private_real_argb_colors array in .rdata.
 */
#include "../headers/real_argb_color.h"

extern const real_argb_color private_real_argb_colors[17]; /* 0x82113F08 (.rdata) */

const real_argb_color *global_real_argb_violet = &private_real_argb_colors[16];
