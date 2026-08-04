/* hcex_resize_render_targets @0x823EF858 — resize the on-demand HALO render-target textures (slots 3..8) to
 * the current screen resolution and re-wire their D3D texture + top-level surface into global_render_targets.
 * Slots 0..2 (full-screen 8888 targets) and slot 9 (depth buffer) are left alone. vidDriver / txmD3D_TEX are
 * ws-engine boundaries.
 *
 * CAVEAT: the disasm returns whatever is left in r3 (the last texture-wrapper slot examined); no caller
 * consumes the value, so the declared txmD3D_TEX* return is a decompiler leftover, reproduced as such. */

#include "../headers/hcex/hcex_resize_render_targets_boundary.h"  /* vidDriver, hcxe_global_render_targets_tex, txmD3D_TEX__Resize */
#include "../headers/rasterizer_render_target.h"                 /* global_render_targets, D3DTexture_GetSurfaceLevel */

extern "C" txmD3D_TEX *hcex_resize_render_targets(void)
{
    rasterizer_render_target *render_target = &global_render_targets[3];
    txmD3D_TEX **tex_slot = &hcxe_global_render_targets_tex[3];
    txmD3D_TEX  *tex = 0;

    do
    {
        tex = *tex_slot;
        if ( tex )
        {
            txmD3D_TEX__Resize(tex, vidDriver->pMode->sx, vidDriver->pMode->sy);

            D3DTexture *d3d_texture = txmD3D_TEX__GetD3DTex(*tex_slot);
            render_target->texture = d3d_texture;
            render_target->surface = D3DTexture_GetSurfaceLevel(d3d_texture, 0);
        }
        ++tex_slot;
        ++render_target;
    }
    while ( tex_slot < &hcxe_global_render_targets_tex[9] );

    return tex;
}
