/* rasterizer_dx9_set_decal_zbias @0x8369D498 — push the configured decal depth-bias / slope-scaled depth-bias
 * onto the device so decals render slightly in front of the surfaces they sit on. The config values are floats
 * passed to the device as their raw dword bit patterns (the Xbox DepthBias state is a float-as-dword). */

#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"


extern void D3DDevice_SetRenderState_DepthBias(D3DDevice *device, int bias);
extern void D3DDevice_SetRenderState_SlopeScaleDepthBias(D3DDevice *device, int bias);

void rasterizer_dx9_set_decal_zbias(void)
{
    D3DDevice_SetRenderState_DepthBias(global_d3d_device, *(const int *)&cf_DecalZBiasValue);
    D3DDevice_SetRenderState_SlopeScaleDepthBias(global_d3d_device, *(const int *)&cf_DecalSlopeZBiasValue);
}
