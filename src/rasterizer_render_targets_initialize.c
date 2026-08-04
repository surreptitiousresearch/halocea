/* rasterizer_render_targets_initialize @ 0x836F3550
   Captures the current back buffer as render target 0, mirrors its dimensions
   into the auxiliary render targets (1, 2, 9) and forces target 1 to ARGB8888,
   then hands off to hcex_init_render_targets for the hcex-side allocations.

   The return value reproduces the decompiler's branchless idiom: it yields 1
   when the back-buffer surface was obtained, 0 otherwise (the 0x88760866 is the
   D3DERR failure HRESULT folded into the test). */

#include <stdint.h>
#include "headers/rasterizer_render_target.h"
#include "headers/rasterizer_engine_globals.h"

extern void hcex_init_render_targets(void);

uint8_t rasterizer_render_targets_initialize(void)
{
    _D3DSURFACE_DESC desc;

    global_render_targets[0].surface = D3DDevice_GetRenderTarget(global_d3d_device, 0);
    unsigned int succeeded =
        (((global_render_targets[0].surface != 0 ? 0 : 0x88760866) >> 31) - 1) & 1;

    D3DSurface_GetDesc(global_render_targets[0].surface, &desc);

    global_render_targets[1].format = D3DFMT_A8R8G8B8;
    global_render_targets[0].height = desc.Height;
    global_render_targets[0].format = desc.Format;
    global_render_targets[1].height = desc.Height;
    global_render_targets[9].height = desc.Height;
    global_render_targets[2].height = desc.Height;
    global_render_targets[0].width = desc.Width;
    global_render_targets[1].width = desc.Width;
    global_render_targets[9].width = desc.Width;
    global_render_targets[2].width = desc.Width;

    hcex_init_render_targets();
    return succeeded;
}
