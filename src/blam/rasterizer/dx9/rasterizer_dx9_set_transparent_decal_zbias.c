/* rasterizer_dx9_set_transparent_decal_zbias @0x8369D4E0 — push the configured transparent-decal depth
 * bias into the current D3D device state: constant bias and slope-scaled bias. The two cf_ config globals
 * are floats but the SetRenderState helpers take the value as a raw dword (FloatAsDword), so their bit
 * patterns are passed through unchanged (matches disasm: lwz of the float then bl). */

#include "headers/D3DDevice.h"
#include "headers/blam_data_globals.h"

extern float cf_TransparentDecalZBiasValue;
extern float cf_TransparentDecalSlopeZBiasValue;

extern void D3DDevice_SetRenderState_DepthBias(D3DDevice *pDevice, unsigned int FloatAsDword);
extern void D3DDevice_SetRenderState_SlopeScaleDepthBias(D3DDevice *pDevice, unsigned int FloatAsDword);

void rasterizer_dx9_set_transparent_decal_zbias(void)
{
    D3DDevice_SetRenderState_DepthBias(global_d3d_device,
                                       *(const unsigned int *)&cf_TransparentDecalZBiasValue);
    D3DDevice_SetRenderState_SlopeScaleDepthBias(global_d3d_device,
                                                 *(const unsigned int *)&cf_TransparentDecalSlopeZBiasValue);
}
