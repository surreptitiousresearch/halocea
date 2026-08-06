/* private_real_argb_colors @ 0x82113F08 (.rdata, 272 bytes)  DB decl: const real_argb_color private_real_argb_colors[17];
 * Image bytes (big-endian), reconstructed from the binary .rdata record - 17 x real_argb_color,
 * four IEEE-754 words each, in the order alpha, red, green, blue:
 *   +0x000 [ 0] white      3F800000 3F800000 3F800000 3F800000 = 1.0, 1.0, 1.0, 1.0
 *   +0x010 [ 1] grey       3F800000 3F000000 3F000000 3F000000 = 1.0, 0.5, 0.5, 0.5
 *   +0x020 [ 2] black      3F800000 00000000 00000000 00000000 = 1.0, 0.0, 0.0, 0.0
 *   +0x030 [ 3] red        3F800000 3F800000 00000000 00000000 = 1.0, 1.0, 0.0, 0.0
 *   +0x040 [ 4] green      3F800000 00000000 3F800000 00000000 = 1.0, 0.0, 1.0, 0.0
 *   +0x050 [ 5] blue       3F800000 00000000 00000000 3F800000 = 1.0, 0.0, 0.0, 1.0
 *   +0x060 [ 6] cyan       3F800000 00000000 3F800000 3F800000 = 1.0, 0.0, 1.0, 1.0
 *   +0x070 [ 7] yellow     3F800000 3F800000 3F800000 00000000 = 1.0, 1.0, 1.0, 0.0
 *   +0x080 [ 8] magenta    3F800000 3F800000 00000000 3F800000 = 1.0, 1.0, 0.0, 1.0
 *   +0x090 [ 9] pink       3F800000 3F800000 3ED1EB85 3F333333 = 1.0, 1.0, 0.41, 0.7
 *   +0x0A0 [10] lightblue  3F800000 3EC7AE14 3F147AE1 3F6E147B = 1.0, 0.39, 0.58, 0.93
 *   +0x0B0 [11] orange     3F800000 3F800000 3F000000 00000000 = 1.0, 1.0, 0.5, 0.0
 *   +0x0C0 [12] purple     3F800000 3EE147AE 3D4CCCCD 3EDC28F6 = 1.0, 0.44, 0.05, 0.43
 *   +0x0D0 [13] aqua       3F800000 3F000000 3F800000 3F547AE1 = 1.0, 0.5, 1.0, 0.83
 *   +0x0E0 [14] darkgreen  3F800000 00000000 3EC7AE14 00000000 = 1.0, 0.0, 0.39, 0.0
 *   +0x0F0 [15] salmon     3F800000 3F800000 3F2147AE 3EF5C28F = 1.0, 1.0, 0.63, 0.48
 *   +0x100 [16] violet     3F800000 3F4F5C29 3E051EB8 3F0F5C29 = 1.0, 0.81, 0.13, 0.56
 *
 * The Blam named-colour palette. Nothing indexes it at run time: it exists so that the 34 colour
 * pointers in .data can each hold the address of one element. The 17 global_real_argb_<name>
 * pointers at 0x84176D5C..0x84176D9C hold &private_real_argb_colors[i]; the 17
 * global_real_rgb_<name> pointers at 0x84176DA0..0x84176DE0 hold the address of the same element's
 * rgb sub-object, i.e. element + 4.
 *
 * Element type and count are PROVEN, not inferred. applied_types at 0x82113F08 states
 * `const real_argb_color private_real_argb_colors[17];`. The 34 pointer words decode big-endian to
 * exactly 0x82113F08 + 16*i (argb) and 0x82113F0C + 16*i (rgb) for i = 0..16, in the order tabled
 * above. The object ends where the next .rdata symbol begins:
 * 0x82114018 - 0x82113F08 == 272 == 17 * 16, no padding.
 *
 * Component order is alpha, red, green, blue. real_argb_color is a union whose first member is
 * float n[4]; its DB channel view ($A1D52E832813CCBC7C7218CE9905C4B4) puts red at +0x04, green at
 * +0x08 and blue at +0x0C, leaving +0x00 as alpha, and its DB alpha/rgb view
 * ($4B4036038E783B5674E58077268CAECC) is { float alpha; real_rgb_color rgb; }. Corroborated by the
 * global_real_rgb pointers landing at element + 4, and by the values themselves: element 2 (black)
 * is { 1, 0, 0, 0 }, which is opaque black only under alpha-first.
 *
 * Every literal below re-encodes to the image's exact big-endian word (round-trip verified).
 *
 * DEVIATION: the `private_` prefix marks the original as file-static, which implies the array and
 * the pointer definitions shared one translation unit. src/data authors one symbol per TU, so the
 * array is given external linkage here to let the pointer TUs take element addresses; the 272-byte
 * object itself is reproduced byte-for-byte.
 */
#include "../headers/real_argb_color.h"

const real_argb_color private_real_argb_colors[17] =
{
    { { 1.0f,   1.0f,   1.0f,   1.0f   } }, /* white      [ 0] @ 0x82113F08 */
    { { 1.0f,   0.5f,   0.5f,   0.5f   } }, /* grey       [ 1] @ 0x82113F18 */
    { { 1.0f,   0.0f,   0.0f,   0.0f   } }, /* black      [ 2] @ 0x82113F28 */
    { { 1.0f,   1.0f,   0.0f,   0.0f   } }, /* red        [ 3] @ 0x82113F38 */
    { { 1.0f,   0.0f,   1.0f,   0.0f   } }, /* green      [ 4] @ 0x82113F48 */
    { { 1.0f,   0.0f,   0.0f,   1.0f   } }, /* blue       [ 5] @ 0x82113F58 */
    { { 1.0f,   0.0f,   1.0f,   1.0f   } }, /* cyan       [ 6] @ 0x82113F68 */
    { { 1.0f,   1.0f,   1.0f,   0.0f   } }, /* yellow     [ 7] @ 0x82113F78 */
    { { 1.0f,   1.0f,   0.0f,   1.0f   } }, /* magenta    [ 8] @ 0x82113F88 */
    { { 1.0f,   1.0f,   0.41f,  0.7f   } }, /* pink       [ 9] @ 0x82113F98 */
    { { 1.0f,   0.39f,  0.58f,  0.93f  } }, /* lightblue  [10] @ 0x82113FA8 */
    { { 1.0f,   1.0f,   0.5f,   0.0f   } }, /* orange     [11] @ 0x82113FB8 */
    { { 1.0f,   0.44f,  0.05f,  0.43f  } }, /* purple     [12] @ 0x82113FC8 */
    { { 1.0f,   0.5f,   1.0f,   0.83f  } }, /* aqua       [13] @ 0x82113FD8 */
    { { 1.0f,   0.0f,   0.39f,  0.0f   } }, /* darkgreen  [14] @ 0x82113FE8 */
    { { 1.0f,   1.0f,   0.63f,  0.48f  } }, /* salmon     [15] @ 0x82113FF8 */
    { { 1.0f,   0.81f,  0.13f,  0.56f  } }, /* violet     [16] @ 0x82114008 */
};
