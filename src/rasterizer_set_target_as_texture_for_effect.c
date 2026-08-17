/* rasterizer_set_target_as_texture_for_effect @0x836F3898 */
/* rasterizer_set_target_as_texture_for_effect 0x836F3898 — bind one of the global render targets as a
 * texture input on a pixel-shader effect. `target` selects which render target (0..9, else none) and
 * `stage` selects which of the effect's texture parameter handles receives it. */

#include <stdint.h>
#include "headers/rasterizer_render_target.h"
#include "headers/rasterizer_dx9_shader_table.h"

/* max_mipmap is dead in this build: r5 is clobbered (li r5,0 @836F389C) before ever being read. */
void rasterizer_set_target_as_texture_for_effect(int16_t stage, int16_t target, int16_t max_mipmap,
        rasterizer_dx9_shader *dxeffect_shader)
{
    D3DTexture *texture = 0;

    /* disasm 836F38A0/836F38A8: two signed compares (bge 0xA / blt 0), not one unsigned compare */
    if ( target >= 0 && target < 10 )
        texture = global_render_targets[target].texture;

    /* C boundary wrapper for opaque COM object */
    ID3DXEffect_SetTexture(dxeffect_shader->effect, dxeffect_shader->texture[stage], texture);
}
