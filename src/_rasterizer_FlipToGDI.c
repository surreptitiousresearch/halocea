/* _rasterizer_FlipToGDI @0x8369C1F8 — present the back buffer when running fullscreen with a
 * live D3D device, clear the in-scene flag, and consume the pending-flip request. */

#include "headers/rasterizer_engine_globals.h"

extern unsigned char InsideScene;
extern void D3DDevice_Present(D3DDevice *device);

void rasterizer_FlipToGDI(void)
{
    if ( use_fullscreen && global_d3d_device )
    {
        InsideScene = 0;
        if ( flip_status )
        {
            D3DDevice_Present(global_d3d_device);
            flip_status = 0;
        }
    }
}
