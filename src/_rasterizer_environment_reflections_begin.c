/* _rasterizer_environment_reflections_begin @0x837A7C08 */
/* _rasterizer_environment_reflections_begin 0x837A7C08 — configure the D3D pipeline for the environment
 * reflection pass: additive blend (src 0xA / dest ONE), depth-tested (less-equal, no z-write), alpha test off,
 * then the four reflection/gel sampler stages. No-op in a special drawing mode or when environment reflections
 * are disabled.
 *
 * DEVIATION: the decompiler flagged this ("local variable allocation has failed") and emitted the inlined
 * sampler setters as raw GPU fetch-constant register pokes; restored to D3DDevice_SetSamplerState_*_Inline
 * decoded from those writes (field 0=wrap, 2=clamp). The MagFilter values the decompiler garbled (DWORD1/HIDWORD
 * of overlapped temporaries) are all 1 (point) per the disassembly. There are no texture binds. The inferred
 * int return is the leftover pending-mask register; the function is void. */

#include "headers/rasterizer_debug_options_struct.h"
#include "headers/d3d_render_boundary.h"
#include "headers/_D3DTEXTUREFILTERTYPE.h"
#include "headers/_D3DBLENDOP.h"
#include "headers/_D3DCULL.h"
#include "headers/_D3DBLEND.h"
#include "headers/_D3DTEXTUREADDRESS.h"
#include "headers/_D3DCMPFUNC.h"
#include "headers/blam_data_globals.h"


extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int mode);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_BlendOp(D3DDevice *device, unsigned int op);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZFunc(D3DDevice *device, unsigned int func);
extern void D3DDevice_SetRenderState_ZWriteEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressW_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
                                                            unsigned int value);

void _rasterizer_environment_reflections_begin(void)
{
    if ( rasterizer_debug_options.drawing_mode || !rasterizer_debug_options.draw_environment_reflections )
        return;

    D3DDevice_SetRenderState_CullMode(global_d3d_device, D3DCULL_CCW);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_SrcBlend(global_d3d_device, D3DBLEND_DESTALPHA);
    D3DDevice_SetRenderState_DestBlend(global_d3d_device, D3DBLEND_ONE);
    D3DDevice_SetRenderState_BlendOp(global_d3d_device, D3DBLENDOP_ADD);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_ZFunc(global_d3d_device, D3DCMP_EQUAL);
    D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 0);

    /* Stage 0 — wrap U/V, point filter, separate-Z on */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, D3DTADDRESS_WRAP);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, D3DTADDRESS_WRAP);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 1);
    /* Stage 1 — clamp U/V/W, point mag / non-point min, separate-Z off */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 1, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 1, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, 1, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 1, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 1, D3DTEXF_POINT);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 1, 0);
    /* Stage 2 — clamp U/V/W, point mag / non-point min, separate-Z off */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 2, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 2, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, 2, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 2, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 2, D3DTEXF_POINT);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 2, 0);
    /* Stage 3 — clamp U/V/W, point filter, separate-Z on */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 3, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 3, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, 3, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 3, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 3, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 3, 1);
}
