/* hcex_update_render_targets @0x823F0938 — plural form of hcex_update_render_target: refreshes
 * global_render_targets[1..8].texture from the live HALO render-target textures in one pass,
 * optionally substituting each slot's requested split-screen sub-texture (falling back to the base
 * texture if that split view hasn't been created). txmD3D_TEX / vidDriver are ws-engine boundaries. */

#include "../headers/vid_boundary.h"        /* hcxe_global_render_targets_tex, txmD3D_TEX */
#include "../headers/txm_boundary.h"        /* txmTEXTURE__GetSplitScreenTexture */
#include "../headers/txm_d3d_tex.h"         /* txmD3D_TEX__GetD3DTex */
#include "../headers/rasterizer_render_target.h" /* global_render_targets */

void hcex_update_render_targets(int splitScreenIdx)
{
    for ( int slot = 1; slot < 9; ++slot )
    {
        txmD3D_TEX *tex = hcxe_global_render_targets_tex[slot];
        if ( !tex )
            continue;

        if ( splitScreenIdx != -1 )
        {
            txmD3D_TEX *split = (txmD3D_TEX *)txmTEXTURE__GetSplitScreenTexture((txmTEXTURE *)tex, splitScreenIdx);
            tex = split ? split : hcxe_global_render_targets_tex[slot];
        }

        global_render_targets[slot].texture = txmD3D_TEX__GetD3DTex(tex);
    }
}
