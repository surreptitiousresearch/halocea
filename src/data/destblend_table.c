/* destblend_table @0x8210FAB0 — .rdata lookup table, 36 bytes = 9 x 4.
 * Element width: `slwi r29, r11, 2` + `lwzx r4, r29, r8` at 0x8369C3D0/0x8369C3F4
 * (rasterizer_set_framebuffer_blend_function) — 4-byte stride; blendop_table starts at
 * 0x8210FAD4, so all 9 words belong to this object.
 * Indexed by shader_framebuffer_blend_function; the 9th word is the -1 terminator.
 *   [0] 0x00000007 -> D3DBLEND_INVSRCALPHA  [5] 0x00000001 -> D3DBLEND_ONE
 *   [1] 0x00000000 -> D3DBLEND_ZERO         [6] 0x00000001 -> D3DBLEND_ONE
 *   [2] 0x00000004 -> D3DBLEND_SRCCOLOR     [7] 0x00000007 -> D3DBLEND_INVSRCALPHA
 *   [3] 0x00000001 -> D3DBLEND_ONE          [8] 0xFFFFFFFF -> -1 (terminator)
 *   [4] 0x00000001 -> D3DBLEND_ONE
 */

#include "_D3DBLEND.h"
#include "shader_framebuffer_blend_function.h"

const unsigned int destblend_table[NUMBER_OF_SHADER_FRAMEBUFFER_BLEND_FUNCTIONS + 1] =
{
    D3DBLEND_INVSRCALPHA,  /* alpha_blend         */
    D3DBLEND_ZERO,         /* multiply            */
    D3DBLEND_SRCCOLOR,     /* double_multiply     */
    D3DBLEND_ONE,          /* add                 */
    D3DBLEND_ONE,          /* reverse_subtract    */
    D3DBLEND_ONE,          /* min                 */
    D3DBLEND_ONE,          /* max                 */
    D3DBLEND_INVSRCALPHA,  /* alpha_multiply_add  */
    -1,                    /* 0xFFFFFFFF terminator */
};
