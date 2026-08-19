/* rasterizer_set_target @ 0x836F3680
   Binds a render target and configures the viewport. For the main scene target
   (target 1) the viewport is derived from the camera's viewport_bounds scaled
   to the target's pixel dimensions (reference resolution 1280x720); for other
   targets the full surface is used. Optionally clears color (+depth/stencil for
   targets 1 and 2). */

#include <stdint.h>
#include "headers/rasterizer_render_target.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_engine_globals.h"

/* hcex_set_render_target takes full-width int args (r3/r4 read via mr, no extsh) */
extern void hcex_set_render_target(int new_tgt, int old_tgt);
extern void rasterizer_set_frustum_z(float z_near, float z_far);

void rasterizer_set_target(int16_t target, int16_t mipmap_index, uint32_t background_color,
                           uint8_t clear, uint8_t zbuffer)
{
    short previous_target = global_current_render_target;
    D3DSurface *surface = 0;
    if ( (unsigned int)target <= 9 )
        surface = global_render_targets[target].surface;
    global_current_render_target = target;

    hcex_set_render_target(target, previous_target);
    rasterizer_set_frustum_z(-2.0f, -2.0f);

    _D3DSURFACE_DESC desc;
    D3DSurface_GetDesc(surface, &desc);

    _D3DVIEWPORT9 viewport;
    if ( target == 1 )
    {
        const rectangle2d *vb = &global_window_parameters.camera.viewport_bounds;
        viewport.Y = desc.Height * vb->y0 / 720;
        viewport.X = desc.Width * vb->x0 / 1280;
        viewport.Width  = desc.Width  * (vb->x1 - vb->x0) / 1280;
        viewport.Height = desc.Height * (vb->y1 - vb->y0) / 720;
    }
    else
    {
        viewport.X = 0;
        viewport.Y = 0;
        viewport.Width = desc.Width;
        viewport.Height = desc.Height;
    }
    viewport.MinZ = 0.0f;
    viewport.MaxZ = 1.0f;
    D3DDevice_SetViewport(global_d3d_device, &viewport);

    if ( clear )
    {
        unsigned int clear_flags = 15;            /* color */
        if ( target == 1 || target == 2 )
            clear_flags = 63;                     /* color + depth + stencil */
        /* stencil arg is left uninitialized by the binary; pass 0 */
        D3DDevice_Clear(global_d3d_device, 0, 0, clear_flags, background_color, 1.0f, 0, 0);
    }
}
