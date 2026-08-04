/* hcex_set_render_target @ 0x823F0A00
   Switches the engine video driver's active render target. Unsets the previous
   target's texture; in non-Halo mode it push/pops the driver's render-target
   stack, otherwise it binds the new target's texture (render-state flag 1 for
   targets 0/1, flag 4 for higher targets).

   Note: the binary compares the (non-halo-mode) boolean against the target
   indices directly; reproduced verbatim. */

#include "../headers/vid_boundary.h"

extern int hcex_is_halo_mode(void);

extern "C" void hcex_set_render_target(unsigned int new_tgt, int old_tgt)
{
    int previous = (old_tgt == -1) ? 0 : old_tgt;
    int not_halo_mode = hcex_is_halo_mode() == 0;

    vidDRIVER__UnsetRenderTarget(vidDriver, hcxe_global_render_targets_tex[previous], 0);

    if ( not_halo_mode == previous )
        vidDRIVER__PushRenderTarget(vidDriver);

    if ( not_halo_mode == (int)new_tgt )
    {
        vidDRIVER__PopRenderTarget(vidDriver);
    }
    else
    {
        txmD3D_TEX *tex = hcxe_global_render_targets_tex[new_tgt];
        if ( new_tgt < 2 )
            vidDRIVER__SetRenderTarget(vidDriver, tex, 0, (void *)1);
        else
            vidDRIVER__SetRenderTarget(vidDriver, tex, 0, (void *)4);
    }
}
