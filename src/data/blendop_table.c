/* blendop_table @0x8210FAD4 — .rdata lookup table, 36 bytes = 9 x 4.
 * Element width: `slwi r29, r11, 2` + `lwzx` off the blendop_table base at 0x8369C3CC/0x8369C3D0
 * (rasterizer_set_framebuffer_blend_function) — 4-byte stride; the next .rdata name is the
 * string at 0x8210FAF8, i.e. exactly 9 words.
 * Indexed by shader_framebuffer_blend_function; the 9th word is the -1 terminator.
 *   [0..3] 0x00000000 -> D3DBLENDOP_ADD     [6] 0x00000003 -> D3DBLENDOP_MAX
 *   [4]    0x00000004 -> D3DBLENDOP_REVSUBTRACT
 *   [5]    0x00000002 -> D3DBLENDOP_MIN     [7] 0x00000000 -> D3DBLENDOP_ADD
 *                                           [8] 0xFFFFFFFF -> -1 (terminator)
 */

#include "_D3DBLENDOP.h"
#include "shader_framebuffer_blend_function.h"

const int blendop_table[NUMBER_OF_SHADER_FRAMEBUFFER_BLEND_FUNCTIONS + 1] =
{
    D3DBLENDOP_ADD,         /* alpha_blend         */
    D3DBLENDOP_ADD,         /* multiply            */
    D3DBLENDOP_ADD,         /* double_multiply     */
    D3DBLENDOP_ADD,         /* add                 */
    D3DBLENDOP_REVSUBTRACT, /* reverse_subtract    */
    D3DBLENDOP_MIN,         /* min                 */
    D3DBLENDOP_MAX,         /* max                 */
    D3DBLENDOP_ADD,         /* alpha_multiply_add  */
    -1,                     /* 0xFFFFFFFF terminator */
};
