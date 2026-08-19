/* rasterizer_dx9_reset_zbias @0x8369D528 — clear the decal depth-bias state back to zero. */

#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"


extern void D3DDevice_SetRenderState_DepthBias(D3DDevice *device, int bias);
extern void D3DDevice_SetRenderState_SlopeScaleDepthBias(D3DDevice *device, int bias);

void rasterizer_dx9_reset_zbias(void)
{
    D3DDevice_SetRenderState_DepthBias(global_d3d_device, 0);
    D3DDevice_SetRenderState_SlopeScaleDepthBias(global_d3d_device, 0);
}
