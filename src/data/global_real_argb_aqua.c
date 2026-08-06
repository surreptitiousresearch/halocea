/* global_real_argb_aqua @ 0x84176D90 (.data, 4 bytes)  DB decl: const real_argb_color *global_real_argb_aqua;
 * Image bytes (big-endian), reconstructed from the binary .data record:
 *   +0x0000 = 0x82113FD8 -> &private_real_argb_colors[13]   (0x82113F08 + 16*13)
 *
 * That element decodes to { alpha 1.0, red 0.5, green 1.0, blue 0.83 } = aqua.
 * NOTE, since it reads as a contradiction otherwise: this is the aquamarine tint, not the web
 * colour "aqua". Web aqua is { 0, 1, 1 }, which is element 6 of this same array under the name
 * cyan. The bytes are unambiguous and are reproduced as found.
 * There is no private 16-byte constant behind this pointer: all 17 global_real_argb_<name> pointers
 * address elements of the single 272-byte private_real_argb_colors array in .rdata.
 */
#include "../headers/real_argb_color.h"

extern const real_argb_color private_real_argb_colors[17]; /* 0x82113F08 (.rdata) */

const real_argb_color *global_real_argb_aqua = &private_real_argb_colors[13];
