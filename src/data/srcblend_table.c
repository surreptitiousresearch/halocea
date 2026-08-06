/* srcblend_table @ 0x8210FA8C (.rdata, 36 bytes)
 * DB applied_types: const unsigned int srcblend_table[9];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000 = 0x00000006
 *   +0x0004 = 0x00000008
 *   +0x0008 = 0x00000008
 *   +0x000C = 0x00000001
 *   +0x0010 = 0x00000001
 *   +0x0014 = 0x00000001
 *   +0x0018 = 0x00000001
 *   +0x001C = 0x00000001
 *   +0x0020 = 0xFFFFFFFF
 * .rdata lookup table, 36 bytes = 9 x 4.
 * Element width: `slwi r29, r11, 2` + `lwzx r4, r29, r9` at 0x8369C3D0/0x8369C3E4
 * (rasterizer_set_framebuffer_blend_function) — 4-byte stride, so 9 words, and destblend_table
 * starts at 0x8210FAB0 so there is no trailing pad to attribute.
 * Indexed by shader_framebuffer_blend_function; the 9th word is the -1 terminator.
 *   [0] 0x00000006 -> D3DBLEND_SRCALPHA    [5] 0x00000001 -> D3DBLEND_ONE
 *   [1] 0x00000008 -> D3DBLEND_DESTCOLOR   [6] 0x00000001 -> D3DBLEND_ONE
 *   [2] 0x00000008 -> D3DBLEND_DESTCOLOR   [7] 0x00000001 -> D3DBLEND_ONE
 *   [3] 0x00000001 -> D3DBLEND_ONE         [8] 0xFFFFFFFF -> -1 (terminator)
 *   [4] 0x00000001 -> D3DBLEND_ONE
 * /
 */
#include "_D3DBLEND.h"
#include "shader_framebuffer_blend_function.h"

const unsigned int srcblend_table[NUMBER_OF_SHADER_FRAMEBUFFER_BLEND_FUNCTIONS + 1] =
{
    D3DBLEND_SRCALPHA,   /* alpha_blend         : src*srcA + dst*(1-srcA) */
    D3DBLEND_DESTCOLOR,  /* multiply            : src*dst */
    D3DBLEND_DESTCOLOR,  /* double_multiply     : src*dst + dst*src */
    D3DBLEND_ONE,        /* add                 */
    D3DBLEND_ONE,        /* reverse_subtract    */
    D3DBLEND_ONE,        /* min                 */
    D3DBLEND_ONE,        /* max                 */
    D3DBLEND_ONE,        /* alpha_multiply_add  : src + dst*(1-srcA) */
    -1,                  /* 0xFFFFFFFF terminator */
};
