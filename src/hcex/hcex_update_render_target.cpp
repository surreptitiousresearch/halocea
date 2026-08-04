/* hcex_update_render_target @0x823EF7F0 — refresh global_render_targets[textureId].texture from the
 * live HALO render-target texture, optionally substituting the requested split-screen sub-texture
 * (falling back to the base texture if that split view hasn't been created). txmD3D_TEX / vidDriver
 * are ws-engine boundaries. */

#include "../headers/vid_boundary.h"        /* hcxe_global_render_targets_tex, txmD3D_TEX */
#include "../headers/txm_boundary.h"        /* txmTEXTURE__GetSplitScreenTexture */
#include "../headers/txm_d3d_tex.h"         /* txmD3D_TEX__GetD3DTex */
#include "../headers/rasterizer_render_target.h" /* global_render_targets */

void hcex_update_render_target(int textureId, int splitScreenIdx)
{
    txmD3D_TEX *tex = hcxe_global_render_targets_tex[textureId];
    if ( !tex )
        return;

    if ( splitScreenIdx != -1 )
    {
        txmD3D_TEX *split = (txmD3D_TEX *)txmTEXTURE__GetSplitScreenTexture((txmTEXTURE *)tex, splitScreenIdx);
        tex = split ? split : hcxe_global_render_targets_tex[textureId];
    }

    global_render_targets[textureId].texture = txmD3D_TEX__GetD3DTex(tex);
}
