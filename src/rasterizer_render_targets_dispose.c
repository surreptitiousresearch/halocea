/* rasterizer_render_targets_dispose @0x836F35F0 — release every render target's D3D surface.
 * DEVIATION: the decompiler bounds the loop by `&global_vector_palette[1]`, an address that happens to
 * coincide with the end of global_render_targets[10] due to link-time global layout, not a real
 * relationship between the two arrays; restored to a clean indexed loop over the 10-element array. */

#include "headers/rasterizer_render_target.h"
#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"

extern unsigned int D3DResource_Release(D3DResource *resource);

D3DResource *rasterizer_render_targets_dispose(void)
{
    D3DResource *result = NULL;

    for (int i = 0; i < 10; ++i)
    {
        if (global_render_targets[i].surface)
        {
            result = (D3DResource *)D3DResource_Release((D3DResource *)global_render_targets[i].surface);
            global_render_targets[i].surface = NULL;
        }
    }

    global_current_render_target = -1;
    return result;
}
