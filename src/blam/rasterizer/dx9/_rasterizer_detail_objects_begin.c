/* _rasterizer_detail_objects_begin @0x83712F18 */
#include <stdint.h>
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/d3d_boundary.h"
#include "headers/d3d_render_boundary.h"
#include "headers/_D3DTEXTUREFILTERTYPE.h"
#include "headers/_D3DBLENDOP.h"
#include "headers/_D3DCULL.h"
#include "headers/_D3DBLEND.h"
#include "headers/_D3DTEXTUREADDRESS.h"
#include "headers/_D3DCMPFUNC.h"
#include "headers/blam_data_globals.h"

extern int16_t main_get_window_count(void);

extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MipFilter_Inline(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);

extern D3DVertexBuffer *hcex_stat_vbuf_get(void *static_vbuf);

void _rasterizer_detail_objects_begin(void)
{
    if (!rasterizer_debug_options.draw_detail_objects || main_get_window_count() > 1)
        return;

    D3DDevice_SetRenderState_CullMode(global_d3d_device, D3DCULL_NONE);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_SrcBlend(global_d3d_device, D3DBLEND_SRCALPHA);
    D3DDevice_SetRenderState_DestBlend(global_d3d_device, D3DBLEND_INVSRCALPHA);
    D3DDevice_SetRenderState_BlendOp(global_d3d_device, D3DBLENDOP_ADD);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_ZFunc(global_d3d_device, D3DCMP_LESSEQUAL);
    D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 0);

    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MipFilter_Inline(global_d3d_device, 0, D3DTEXF_LINEAR);

    /* screen-facing detail-object billboard basis + UV corner offsets, uploaded as a 6-vector4 block */
    float constants[24];
    constants[0]  = 255.00999f;
    constants[1]  = 0.0f;
    constants[2]  = 1.0f;
    constants[3]  = 1.0f;
    constants[4]  = 8.0f;
    constants[5]  = 8.0f;
    constants[6]  = 8.0f;
    constants[7]  = rasterizer_debug_options.detail_object_screen_facing_offset_multiplier;
    constants[8]  = 1.0f;
    constants[9]  = 1.0f;
    constants[10] = 0.5f;
    constants[11] = 0.0f;
    constants[12] = 0.0f;
    constants[13] = 1.0f;
    constants[14] = -0.5f;
    constants[15] = 0.0f;
    constants[16] = 0.0f;
    constants[17] = 0.0f;
    constants[18] = -0.5f;
    constants[19] = 1.0f;
    constants[20] = 1.0f;
    constants[21] = 0.0f;
    constants[22] = 0.5f;
    constants[23] = 1.0f;

    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, constants, 6, 3ULL << 59);

    D3DVertexBuffer *vertex_buffer = hcex_stat_vbuf_get(local_d3d_vertex_buffer_0);
    D3DDevice_SetStreamSource(global_d3d_device, 0, vertex_buffer, local_d3d_vertex_buffer_ofs, 0x14, 1);
    D3DDevice_SetPixelShader(global_d3d_device, NULL);

    SetTextureStageStateSmart(0, D3DTSS_COLOROP,   4);
    SetTextureStageStateSmart(0, D3DTSS_COLORARG1, 2);
    SetTextureStageStateSmart(0, D3DTSS_COLORARG2, 0);
    SetTextureStageStateSmart(0, D3DTSS_ALPHAOP,   4);
    SetTextureStageStateSmart(0, D3DTSS_ALPHAARG1, 2);
    SetTextureStageStateSmart(0, D3DTSS_ALPHAARG2, 0);
    SetTextureStageStateSmart(1, D3DTSS_COLOROP,   1);
    SetTextureStageStateSmart(1, D3DTSS_ALPHAOP,   1);
}
