/* _rasterizer_screen_effect_ps @0x83686428 — draw the full-screen cinematic ("filth") post-process
 * with the pixel-shader / ID3DXEffect path (rasterizer shader 114). Runs a ping-pong multi-pass
 * convolution/desaturation/light-enhancement filter and the analog-video (scanline+noise+overbright)
 * overlay, compositing a full-screen NDC quad each pass. total_passes = 2 * (convolution_extra_passes+1);
 * passes alternate render targets (odd: src RT2 -> tgt RT1, even: src RT1 -> tgt RT2, single pass ->
 * back buffer), the final pass composites to the window's rasterizer target.
 *
 * Reconstructed from the decompiler draft (Hex-Rays "local variable allocation has failed") plus full
 * per-instruction disasm decode (0x83686428-0x83686FBC). DEVIATIONS, all disasm-verified:
 * - The decompiler fused r22 (the constant PendingMask 1<<63, disasm `extldi r22,r10,64,63`) and r21
 *   (the pass-loop counter) into one 64-bit pseudo-reg `v8`. So `(v8 & 0x100000000)` is really
 *   `pass_index & 1` (odd/even), `HIDWORD(v8)` is `pass_index`, and `v8 >> (stage+32)` is the per-stage
 *   sampler dirty bit 1<<(31-stage) — which is internal to the inlined D3DDevice_SetSamplerState_*_Inline
 *   accessors, so the raw m_Constants.Fetch/m_Pending.m_Mask pokes are reversed to named helper CALLS
 *   (established sun_glow/water/blip convention; the D3DDevice struct is NOT reproduced).
 * - width/height: `rasterizer_render_target_get_dimension(source_rt, &width, &height)` writes width to
 *   var_12C and height to var_130 (disasm 0x83686630-44); the set_texture_transforms call passes them
 *   in the correct (width,height) order (0x83686B44/4C) despite the decompiler's v50/HIDWORD(v50) puns.
 * - Both loop-B stage switches (0x836869B4 and 0x83686958) branch case 1/2/3 -> LABEL_56 (bind source RT
 *   to that stage); the decompiler rendered the first as a plain `break` — it is a `goto LABEL_56`
 *   (disasm `b loc_83686A28`). default -> LABEL_59 (render).
 * - BeginPass subpass index (v9, r19): video -> pass_index; convolution_type!=0 -> 0; convolution_type==0
 *   single pass -> pass_index(=0); convolution_type==0 multipass pass 0 -> 1; pass 1 with mask -> 2;
 *   otherwise the pass draws nothing (skip to the loop increment).
 * - overbright table {1,4,2} indexed by video_overbright_mode is built on the stack as v59=1 /
 *   v60=0x200000004LL (two ints 4,2); reproduced as a local array.
 * - the even-multipass vertical-texcoord remap uses 0.0013888889 == 1/720 (const __real_3ab60b61).
 * - ID3DXEffect method calls use the simplified boundary vtable names (Begin/BeginPass/EndPass/End/
 *   SetVector) per the reconstructed corpus; the technique is set via rasterizer_set_technique. */

#include <stdint.h>
#include "headers/rasterizer_screen_effect_parameters.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/bitmap_data.h"
#include "headers/d3d_boundary.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/rasterizer_target.h"
#include "headers/blam_data_globals.h"


/* screen-effect config gates */

/* ID3DXEffect technique handles for the "filth" shader (114) */

/* Definition returns/takes struct cinematic_screen_effect_globals *; the caller reinterprets it as the
 * embedded rasterizer_screen_effect_parameters block (forward-declared tag; pointer-only use). */
struct cinematic_screen_effect_globals;
#include "headers/bitmap_data.h"
extern struct cinematic_screen_effect_globals * rasterizer_screen_effect_get_cinematic_parameters(struct cinematic_screen_effect_globals *parameters);
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
        const void *vertices, unsigned int vertex_stride);
extern void rasterizer_set_target(int16_t target, int16_t mipmap_index, uint32_t background_color, uint8_t clear, uint8_t zbuffer);
extern void rasterizer_render_target_get_dimension(int16_t target, unsigned int *width, unsigned int *height);
extern void rasterizer_set_target_as_texture_for_effect(int16_t stage, int16_t target, int16_t max_mipmap, rasterizer_dx9_shader *dxeffect_shader);
extern int rasterizer_set_texture_bitmap_data_for_effect(int16_t stage, bitmap_data *bitmap, rasterizer_dx9_shader *dxeffect_shader);
extern void rasterizer_screen_effect_set_texture_transforms(const rasterizer_screen_effect_parameters *parameters, int16_t pass, int16_t passes, unsigned int viewport_width, unsigned int viewport_height, uint8_t ignore_stage_0);
extern long rasterizer_set_technique(ID3DXEffect *effect, unsigned int technique);

extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
        unsigned int value);

void _rasterizer_screen_effect_ps(const rasterizer_screen_effect_parameters *parameters)
{
    const rasterizer_screen_effect_parameters *cinematic_parameters = parameters;

    if ( !hcex_off_cine_screen_effect || !debug_no_drawing )
        cinematic_parameters = (const rasterizer_screen_effect_parameters *)
                rasterizer_screen_effect_get_cinematic_parameters(
                        (struct cinematic_screen_effect_globals *)parameters);

    if ( !cinematic_parameters
      || !(cinematic_parameters->convolution_type
        || cinematic_parameters->convolution_mask
        || cinematic_parameters->filter_light_enhancement_intensity > 0.0f
        || cinematic_parameters->filter_desaturation_intensity > 0.0f
        || cinematic_parameters->video_on) )
        return;

    if ( !rasterizer_debug_options.screen_effects_enabled )
        return;
    if ( global_window_parameters.rasterizer_target != _rasterizer_target_render_primary )
        return;

    int16_t total_passes = 2 * (cinematic_parameters->convolution_extra_passes + 1);
    rasterizer_dx9_shader *shader = rasterizer_shader_select(_dxshader_screen_effect);

    /* full-screen NDC quad, opaque white */
    vertex_data[0].position.n[0] = -1.0f; vertex_data[0].position.n[1] = -1.0f; vertex_data[0].color = -1;
    vertex_data[1].position.n[0] =  1.0f; vertex_data[1].position.n[1] = -1.0f; vertex_data[1].color = -1;
    vertex_data[2].position.n[0] =  1.0f; vertex_data[2].position.n[1] =  1.0f; vertex_data[2].color = -1;
    vertex_data[3].position.n[0] = -1.0f; vertex_data[3].position.n[1] =  1.0f; vertex_data[3].color = -1;
    vertex_data[0].position.n[2] = 0.0f; vertex_data[1].position.n[2] = 0.0f;
    vertex_data[2].position.n[2] = 0.0f; vertex_data[3].position.n[2] = 0.0f;

    if ( !shader || !shader->effect )
        return;

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_screen));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_convolution));

    for ( int16_t pass_index = 0; pass_index < total_passes; ++pass_index )
    {
        /* ping-pong render targets by pass parity (single pass -> back buffer) */
        int16_t source_rt, target_rt;
        if ( total_passes == 1 )      { source_rt = -1; target_rt = -1; }
        else if ( pass_index & 1 )    { source_rt =  2; target_rt =  1; }
        else                          { source_rt =  1; target_rt =  2; }

        rasterizer_set_target(target_rt, 0, 0, 0, 0);

        unsigned int width, height;
        rasterizer_render_target_get_dimension(source_rt, &width, &height);

        /* quad textured from the source render target in pixel space:
         * v0=(0,h) v1=(w,h) v2=(w,0) v3=(0,0) */
        vertex_data[0].texcoord.n[0] = 0.0f;         vertex_data[0].texcoord.n[1] = (float)height;
        vertex_data[1].texcoord.n[0] = (float)width; vertex_data[1].texcoord.n[1] = (float)height;
        vertex_data[2].texcoord.n[0] = (float)width; vertex_data[2].texcoord.n[1] = 0.0f;
        vertex_data[3].texcoord.n[0] = 0.0f;         vertex_data[3].texcoord.n[1] = 0.0f;

        if ( total_passes > 1 && !(pass_index & 1) )
        {
            /* even multi-pass: remap the vertical texcoords to this viewport's y-sub-region (720 rows) */
            int16_t viewport_top    = global_window_parameters.camera.viewport_bounds.n[0];
            int16_t viewport_bottom = global_window_parameters.camera.viewport_bounds.n[2];
            vertex_data[2].texcoord.n[1] = ((float)height * (float)viewport_top) * (float)0.0013888889;
            vertex_data[3].texcoord.n[1] = vertex_data[2].texcoord.n[1];
            vertex_data[0].texcoord.n[1] = ((float)height * (float)viewport_bottom) * (float)0.0013888889;
            vertex_data[1].texcoord.n[1] = vertex_data[0].texcoord.n[1];
        }

        int stage;

        if ( (pass_index & 1) && cinematic_parameters->video_on )
        {
            /* odd video pass: composite the accumulated source RT + scanline map + noise map */
            rasterizer_set_target_as_texture_for_effect(0, source_rt, 0, shader);
            D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, 2);
            D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, 2);
            D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, 0);
            D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, 0);
            D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 0);

            rasterizer_set_texture_bitmap_data_for_effect(1, cinematic_parameters->video_scanline_map, shader);
            D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 1, 2);
            D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 1, 2);
            D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 1, 0);
            D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 1, 0);
            D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 1, 0);

            rasterizer_set_texture_bitmap_data_for_effect(2, cinematic_parameters->video_noise_map, shader);
            D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 2, 0); /* noise wraps */
            D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 2, 0);
            D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 2, 1);
            D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 2, 1);
            D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 2, 0);
        }
        else
        {
            /* per-stage source-texture bind loop (decompiler loop B, stages 0..3) */
            stage = 0;
            while ( 1 )
            {
                if ( cinematic_parameters->convolution_type )
                {
                    if ( cinematic_parameters->convolution_mask )
                    {
                        if ( stage == 0 )
                            rasterizer_set_texture_bitmap_data_for_effect(0, cinematic_parameters->convolution_mask,
                                    shader);
                        else if ( stage <= 3 )
                            rasterizer_set_target_as_texture_for_effect((int16_t)stage, source_rt, 0, shader);
                        else
                            goto render;
                        goto stage_samplers;
                    }
                    /* convolution but no mask: bind the source RT to all four stages once (stage 0) */
                    if ( stage != 0 )
                        goto render;
                    rasterizer_set_target_as_texture_for_effect(0, source_rt, 0, shader);
                    rasterizer_set_target_as_texture_for_effect(1, source_rt, 0, shader);
                    rasterizer_set_target_as_texture_for_effect(2, source_rt, 0, shader);
                    rasterizer_set_target_as_texture_for_effect(3, source_rt, 0, shader);
                    goto stage_samplers;
                }
                /* convolution_type == 0 */
                if ( total_passes == 1 )
                {
                    if ( stage == 0 )
                        rasterizer_set_texture_bitmap_data_for_effect(0, cinematic_parameters->convolution_mask,
                                shader);
                    else
                        goto next_stage;
                    goto stage_samplers;
                }
                if ( pass_index == 0 )
                {
                    /* first pass: bind the source RT to stage 0 only (decompiler LABEL_34) */
                    if ( stage == 0 )
                        rasterizer_set_target_as_texture_for_effect(0, source_rt, 0, shader);
                    else
                        goto next_stage;
                    goto stage_samplers;
                }
                if ( pass_index != 1 )
                    goto stage_samplers; /* later passes: no new bind, just refresh samplers */
                if ( !cinematic_parameters->convolution_mask )
                {
                    if ( stage == 0 )
                        rasterizer_set_target_as_texture_for_effect(0, source_rt, 0, shader);
                    else
                        goto next_stage;
                    goto stage_samplers;
                }
                if ( stage == 0 )
                    rasterizer_set_texture_bitmap_data_for_effect(0, cinematic_parameters->convolution_mask, shader);
                else if ( stage <= 3 )
                    rasterizer_set_target_as_texture_for_effect((int16_t)stage, source_rt, 0, shader);
                else
                    goto next_stage;

            stage_samplers: /* decompiler LABEL_57 */
                D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, stage, 2);
                D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, stage, 2);
                D3DDevice_SetSamplerState_MagFilter(global_d3d_device, stage, 1);
                D3DDevice_SetSamplerState_MinFilter(global_d3d_device, stage, 1);
                D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, stage, 0);

            next_stage: /* decompiler LABEL_58 */
                if ( ++stage >= 4 )
                    goto render;
            }
        }

    render: /* decompiler LABEL_59 */
        D3DDevice_SetRenderState_CullMode(global_d3d_device, 0);
        D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
        D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
        D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
        D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);

        rasterizer_screen_effect_set_texture_transforms(cinematic_parameters, pass_index, total_passes,
                width, height, 0);

        int subpass = pass_index;
        float effect_vector[4];              /* tint.rgb + desaturation, or overbright.rgb + noise */
        float light_vector[4];               /* light-enhancement intensity, all four lanes */
        unsigned int technique;

        if ( cinematic_parameters->video_on )
        {
            technique = hVideoOn;
            if ( pass_index == 1 )
            {
                /* overbright multiplier {1,4,2}[mode] (stack table v59=1 / v60=0x200000004LL) */
                int overbright_table[3] = { 1, 4, 2 };
                int overbright = overbright_table[cinematic_parameters->video_overbright_mode];
                effect_vector[0] = (float)overbright;
                effect_vector[1] = (float)overbright;
                effect_vector[2] = (float)overbright;
                float noise = cinematic_parameters->video_noise_intensity;
                if ( noise < 0.0f )
                    noise = 0.0f;
                else if ( noise > 1.0f )
                    noise = 1.0f;
                effect_vector[3] = noise;

                if ( shader->constants )
                    shader->effect->lpVtbl->SetVector(shader->effect, shader->constants[0],
                            (const D3DXVECTOR4 *)effect_vector);

                D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
                D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 1);
                D3DDevice_SetRenderState_DestBlend(global_d3d_device, 0);
                D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0);
            }
            rasterizer_set_technique(shader->effect, technique);
        }
        else
        {
            effect_vector[0] = cinematic_parameters->filter_desaturation_tint.n[0];
            effect_vector[1] = cinematic_parameters->filter_desaturation_tint.n[1];
            effect_vector[2] = cinematic_parameters->filter_desaturation_tint.n[2];
            effect_vector[3] = cinematic_parameters->filter_desaturation_intensity;
            light_vector[0] = cinematic_parameters->filter_light_enhancement_intensity;
            light_vector[1] = cinematic_parameters->filter_light_enhancement_intensity;
            light_vector[2] = cinematic_parameters->filter_light_enhancement_intensity;
            light_vector[3] = cinematic_parameters->filter_light_enhancement_intensity;

            if ( cinematic_parameters->convolution_type )
            {
                subpass = 0;
                int16_t last_pass = total_passes - 1;

                if ( !cinematic_parameters->convolution_mask )
                {
                    if ( pass_index != last_pass )
                        goto convolved_default; /* LABEL_107 */

                    if ( cinematic_parameters->filter_light_enhancement_uses_convolution_mask )
                    {
                        if ( cinematic_parameters->filter_desaturation_uses_convolution_mask
                          && cinematic_parameters->filter_light_enhancement_intensity > 0.0f
                          && cinematic_parameters->filter_desaturation_intensity > 0.0f )
                        {
                            technique = hVideoOffConvolvedFilterLightAndDesaturation;
                            if ( !technique )                       /* LABEL_106 */
                                goto convolved_default;
                            goto set_technique;
                        }
                        if ( cinematic_parameters->filter_light_enhancement_uses_convolution_mask
                          && cinematic_parameters->filter_light_enhancement_intensity > 0.0f )
                        {
                            technique = hVideoOffConvolvedFilterLight;
                            if ( !technique )
                                goto convolved_default;
                            goto set_technique;
                        }
                    }
                    if ( cinematic_parameters->filter_desaturation_uses_convolution_mask
                      && cinematic_parameters->filter_desaturation_intensity > 0.0f )
                    {
                        technique = hVideoOffConvolvedFilterDesaturation;
                        if ( !technique )
                            goto convolved_default;
                        goto set_technique;
                    }
                convolved_default: /* LABEL_107 */
                    technique = hVideoOffConvolved;
                    goto set_technique;
                }

                /* convolution mask present */
                if ( pass_index != last_pass )
                    goto convolved_mask_default; /* LABEL_91 */

                if ( cinematic_parameters->filter_light_enhancement_uses_convolution_mask )
                {
                    if ( cinematic_parameters->filter_desaturation_uses_convolution_mask
                      && cinematic_parameters->filter_light_enhancement_intensity > 0.0f
                      && cinematic_parameters->filter_desaturation_intensity > 0.0f )
                    {
                        technique = hVideoOffConvolvedMaskFilterLightAndDesaturation;
                        if ( !technique )                           /* LABEL_90 */
                            goto convolved_mask_default;
                        goto set_technique;
                    }
                    if ( cinematic_parameters->filter_light_enhancement_uses_convolution_mask
                      && cinematic_parameters->filter_light_enhancement_intensity > 0.0f )
                    {
                        technique = hVideoOffConvolvedMaskFilterLight;
                        if ( !technique )
                            goto convolved_mask_default;
                        goto set_technique;
                    }
                }
                if ( cinematic_parameters->filter_desaturation_uses_convolution_mask
                  && cinematic_parameters->filter_desaturation_intensity > 0.0f )
                {
                    technique = hVideoOffConvolvedMaskFilterDesaturation;
                    if ( !technique )
                        goto convolved_mask_default;
                    goto set_technique;
                }
            convolved_mask_default: /* LABEL_91 */
                if ( cf_UseAlternateConvolveMask )
                    technique = hVideoOffConvolveMaskThreeStage;
                else
                    technique = hVideoOffConvolvedMask;
            }
            else
            {
                technique = hVideoOffNonConvolved;
                if ( total_passes != 1 )
                {
                    if ( pass_index != 0 )
                    {
                        if ( pass_index != 1 || !cinematic_parameters->convolution_mask )
                            continue; /* nothing to draw this pass (decompiler LABEL_116) */
                        subpass = 2;
                    }
                    else
                    {
                        subpass = 1;
                    }
                }
            }

        set_technique: /* decompiler LABEL_108 */
            rasterizer_set_technique(shader->effect, technique);
            if ( shader->constants )
            {
                shader->effect->lpVtbl->SetVector(shader->effect, shader->constants[0], (const D3DXVECTOR4 *)effect_vector);
                shader->effect->lpVtbl->SetVector(shader->effect, shader->constants[1], (const D3DXVECTOR4 *)light_vector);
            }

            if ( total_passes == 1 )
            {
                D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
                D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 0);
                D3DDevice_SetRenderState_DestBlend(global_d3d_device, 7);
                D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0);
            }
            else if ( pass_index == total_passes - 1 )
            {
                D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
                D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 7);
                D3DDevice_SetRenderState_DestBlend(global_d3d_device, 0);
                D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0);
            }
        }

        /* draw the quad through the selected technique/pass */
        unsigned int passes_out[2];
        shader->effect->lpVtbl->Begin(shader->effect, passes_out, 3);
        shader->effect->lpVtbl->BeginPass(shader->effect, subpass);
        D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4, vertex_data, 0x18);
        shader->effect->lpVtbl->EndPass(shader->effect);
        shader->effect->lpVtbl->End(shader->effect);
    }

    /* restore the window's rasterizer target (with z) */
    rasterizer_set_target(global_window_parameters.rasterizer_target, 0, 0, 0, 1);
}
