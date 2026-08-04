/* _rasterizer_widget_begin 0x83786918 — begin a 2D widget render pass. Uploads a pixel->NDC orthographic
 * projection (built from the active viewport's width/height) into vertex-shader constant registers 0xD..0x11,
 * binds the widget vertex declaration and shader, then configures the pipeline for the requested pass type:
 *   type 5 — the normal widget pass: alpha-blended, point-sampled (mirror addressing when the device supports
 *            it, else wrap), depth-tested with optional z-test/z-write driven by the flags bits; selects the
 *            widget effect, begins pass 0, and resets the cached widget batch state and tint.
 *   type 6 — the lens-flare occlusion-debug pass: opaque, depth-tested; color writes and z-write only when the
 *            occlusion debug option is set; selects the widget effect and begins pass 1.
 *
 * DEVIATION: the inlined sampler address-mode / separate-Z setters were emitted as raw GPU fetch-constant pokes
 * ("local variable allocation has failed"); restored to the D3DDevice_SetSamplerState_*_Inline calls they inline
 * from. The SetVertexShaderConstantFN count/mask were garbled to (0x18000000, width); the disassembly shows
 * count = 5 and PendingMask0 = (uint64)3 << 59. */

#include <stdint.h>
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_engine_globals.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/widget_state.h"
#include "headers/rectangle2d.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/rasterizer_widget_flags.h"
#include "headers/render_widget_type.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/blam_data_globals.h"



extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern void rasterizer_set_stencil_mode(int16_t stencil_mode);
/* DEVIATION: the binary calls _rasterizer_widget_set_tint_factor (0x83786110) directly here, not the
 * rasterizer_widget_set_tint_factor thunk (0x836A63C0) — corrected the call target. */
extern void _rasterizer_widget_set_tint_factor(float tint_factor);

extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
                                                            unsigned int value);
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int mode);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *device, unsigned int factor);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *device, unsigned int factor);
extern void D3DDevice_SetRenderState_BlendOp(D3DDevice *device, unsigned int op);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZFunc(D3DDevice *device, unsigned int func);
extern void D3DDevice_SetRenderState_ZWriteEnable(D3DDevice *device, unsigned int enable);

void _rasterizer_widget_begin(int16_t type, uint16_t flags)
{
    rectangle2d *viewport = &global_window_parameters.camera.viewport_bounds;
    int width = (__int16)(viewport->n[3] - viewport->n[1]);
    int height = (__int16)((unsigned __int16)viewport->n[2] - (unsigned __int16)viewport->n[0]);

    /* pixel -> NDC orthographic projection (vs registers 0xD..0x11) */
    float vsh_constants[20];
    vsh_constants[1] = 0.0f;
    vsh_constants[2] = 0.0f;
    vsh_constants[4] = 0.0f;
    vsh_constants[6] = 0.0f;
    vsh_constants[8] = 0.0f;
    vsh_constants[9] = 0.0f;
    vsh_constants[10] = 1.0f;
    vsh_constants[11] = 0.0f;
    vsh_constants[12] = 0.0f;
    vsh_constants[13] = 0.0f;
    vsh_constants[14] = 0.0f;
    vsh_constants[15] = 1.0f;
    vsh_constants[16] = 0.0f;
    vsh_constants[17] = 0.0f;
    vsh_constants[18] = 0.0f;
    vsh_constants[19] = 1.0f;
    vsh_constants[0] = (((float)1.0 / (float)width) * (float)2.0);
    vsh_constants[3] = ((float)-1.0 - ((float)1.0 / (float)width));
    vsh_constants[5] = (((float)1.0 / (float)height) * (float)-2.0);
    vsh_constants[7] = (((float)1.0 / (float)height) + (float)1.0);
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, vsh_constants, 5, (unsigned __int64)3 << 59);

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_widget));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_widget));

    unsigned int pass_count;
    if ( type == _widget_type_internal_sprite )
    {
        unsigned int z_enable = flags & (1u << _rasterizer_widget_z_enable_bit);
        if ( !supports_occlusion_test )
            z_enable = 1;
        D3DDevice_SetRenderState_CullMode(global_d3d_device, 6);
        D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
        D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
        D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 6);
        D3DDevice_SetRenderState_DestBlend(global_d3d_device, 1);
        D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0);
        D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
        D3DDevice_SetRenderState_ZEnable(global_d3d_device, z_enable);
        D3DDevice_SetRenderState_ZFunc(global_d3d_device, 3);
        D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, (flags >> _rasterizer_widget_z_write_enable_bit) & 1);

        widget_dxeffect_shader = rasterizer_shader_select(_dxshader_widget_sprite);
        widget_dxeffect_shader->effect->lpVtbl->Begin(widget_dxeffect_shader->effect, &pass_count, 3);
        widget_dxeffect_shader->effect->lpVtbl->BeginPass(widget_dxeffect_shader->effect, 0);

        /* stage 0: mirror addressing if supported else wrap, point filter, separate-Z on */
        if ( (global_d3d_caps.TextureAddressCaps & 8) != 0 )
        {
            D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, 3);
            D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, 3);
        }
        else
        {
            D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, 1);
            D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, 1);
        }
        D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, 1);
        D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, 1);
        D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 1);
        rasterizer_set_stencil_mode(0);

        widget_state_new.stage_index = 0;
        widget_state_new.stencil_mode = 0;
        widget_time_stamp = 0;
        widget_state_new.bitmap_group_index = 0;
        widget_state_new.sequence_index = 0;
        widget_state_current.stage_index = 0;
        widget_state_current.bitmap_group_index = 0;
        widget_state_current.sequence_index = 0;
        widget_state_current.stencil_mode = 0;
        _rasterizer_widget_set_tint_factor(1.0f);
    }
    else if ( type == _widget_type_internal_occlusion_test )
    {
        D3DDevice_SetRenderState_CullMode(global_d3d_device, 6);
        D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device,
            rasterizer_debug_options.lens_flare_occlusion_debug == 0 ? 0 : 7);
        D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
        D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
        unsigned int z_enable = 1; /* occlusion-test pass is always depth-tested */
        D3DDevice_SetRenderState_ZEnable(global_d3d_device, z_enable);
        D3DDevice_SetRenderState_ZFunc(global_d3d_device, 3);
        D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, rasterizer_debug_options.lens_flare_occlusion_debug);

        widget_dxeffect_shader = rasterizer_shader_select(_dxshader_widget_sprite);
        widget_dxeffect_shader->effect->lpVtbl->Begin(widget_dxeffect_shader->effect, &pass_count, 3);
        widget_dxeffect_shader->effect->lpVtbl->BeginPass(widget_dxeffect_shader->effect, 1);
    }
}
