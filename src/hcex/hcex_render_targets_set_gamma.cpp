/* hcex_render_targets_set_gamma @0x823EF780 — toggle hardware gamma-correct sampling on the HALO
 * render-target textures (slots 1..8). Only slots 1 and 2 (the full-screen 8888 targets) actually get
 * the caller's `enable` value; every other slot is forced off. txmD3D_TEX is a ws-engine boundary. */

#include "../headers/vid_boundary.h"   /* hcxe_global_render_targets_tex, txmD3D_TEX */
#include "../headers/txm_d3d_tex.h"    /* txmD3D_TEX__EnableGamma */

void hcex_render_targets_set_gamma(int enable)
{
    for ( int slot = 1; slot < 9; ++slot )
    {
        txmD3D_TEX *tex = hcxe_global_render_targets_tex[slot];
        if ( tex )
        {
            int gamma = (slot == 1 || slot == 2) ? enable : 0;
            txmD3D_TEX__EnableGamma(tex, gamma);
        }
    }
}
