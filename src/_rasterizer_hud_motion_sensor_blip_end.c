/* _rasterizer_hud_motion_sensor_blip_end @0x837AA510 — the second half of the motion-sensor (radar)
 * render, run after all blips have been drawn into render target 5 by the blip_begin/blip_draw pair.
 * It composites that accumulated blip target back onto the HUD in three passes, but only when radar
 * debug drawing is on, blip_begin actually started drawing, and both interface bitmaps (groups 7 and
 * 8) have a valid hardware format:
 *   (1) a fixed-function sweep/fade pass into the target using bitmap 7 (skipped once theta > 2.75),
 *   (2) an effect pass that multiplies in bitmap 8 (the range-ring / mask sheet), and
 *   (3) a final effect pass that draws the finished radar disc onto the screen at center_point,
 *       sized by the HUD scale and player-count (84px single / 64px split-screen).
 * On the non-drawing paths it just restores the main render target. Mirror of
 * _rasterizer_hud_motion_sensor_blip_begin; shares its D3D idioms and helpers.
 *
 * DEVIATIONS (verified against disasm_range 0x837AA510..0x837AAD68):
 *  - The inline sampler-state register pokes the decompiler rendered as raw m_Constants.Fetch[0]
 *    .Texture writes with __ROL4__/__ROR4__ masks are the standard *_Inline helpers: AddressU=2,
 *    AddressV=2, MagFilter=1, MinFilter=1 (rlwimi of r29=1), and SeparateZFilterEnable — a 2-bit
 *    field at dword[3] bits[23:24] — set to 2 in blocks 1 and cleared to 0 in blocks 2/3 (confirmed
 *    against the insrwi/rlwimi in _rasterizer_widget_begin / _rasterizer_hud_motion_sensor_blip_begin).
 *  - The vertex-constant count/mask (5, 3ULL<<59) and pixel-constant count/mask (2, 1ULL<<63) and the
 *    screen-projection vertex-constant mask (3ULL<<59) come from disasm; the decompiler garbled the
 *    latter into a viewport-bounds value.
 *  - The phase-3 screen-projection matrix's __int64 packing (v28/v29/v65) is the PPC integer→float
 *    magic-number conversion of the viewport width/height; reconstructed as plain float divides.
 *  - The four vertices per DrawVerticesUP are one flat 24-float buffer (6 floats/vertex: x,y,z,packed
 *    ARGB,u,v), as in the blip_draw sibling. */

#include <stdint.h>
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/bitmap_data.h"
#include "headers/real_point2d.h"
#include "headers/d3d_boundary.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/interface_tag_index.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/blam_data_globals.h"

extern const float vsh_constants__screenproj[]; /* screen-projection matrix constants (boundary global) */

extern int interface_get_tag_index(int16_t interface_tag_index);
extern bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index);
extern char *tag_get_name(int16_t tag_index);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern int rasterizer_set_texture_bitmap_data(int16_t stage, bitmap_data *bitmap);
extern int rasterizer_set_texture_bitmap_data_for_effect(int16_t stage, bitmap_data *bitmap, rasterizer_dx9_shader *dxeffect_shader);
extern void rasterizer_set_target(int16_t target, int16_t mipmap_index, uint32_t background_color, uint8_t clear, uint8_t zbuffer);
extern void rasterizer_set_target_as_texture_for_effect(int16_t stage, int16_t target, int16_t max_mipmap, rasterizer_dx9_shader *dxeffect_shader);
extern int16_t local_player_count(void);
extern float hud_globals_get_scale(uint8_t in_multiplayer);
extern void SetTextureStageStateSmart(unsigned int stage, _D3DTEXTURESTAGESTATETYPE State, unsigned int Value);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void D3DDevice_SetPixelShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
        const void *vertex_data, unsigned int vertex_stride);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
        unsigned int value);

void _rasterizer_hud_motion_sensor_blip_end(const real_point2d *center_point, float theta)
{
    bitmap_data *sweep_bitmap = bitmap_group_try_and_get_bitmap(interface_get_tag_index(_interface_bitmap_motion_sweep), 0);
    bitmap_data *ring_bitmap = bitmap_group_try_and_get_bitmap(interface_get_tag_index(_interface_bitmap_motion_sweep_mask), 0);
    tag_get_name(interface_get_tag_index(_interface_bitmap_motion_sweep));
    tag_get_name(interface_get_tag_index(_interface_bitmap_motion_sweep_mask));

    uint8_t draw_hud_motion_sensor = rasterizer_debug_options.draw_hud_motion_sensor;
    if ( draw_hud_motion_sensor && rasterizer_motion_sensor_begin_said_to_draw )
    {
        if ( _texture_cache_bitmap_get_hardware_format(sweep_bitmap, 0, 1)
          && _texture_cache_bitmap_get_hardware_format(ring_bitmap, 0, 1) )
        {
            float pixel_constant[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
            unsigned int passes[2];

            rasterizer_dx9_shader *shader = rasterizer_shader_select(_dxshader_dyn_geom);

            /* Close the effect pass opened by blip_begin, then set up fixed-function state for the
             * sweep/fade pass into the target. */
            shader->effect->lpVtbl->EndPass(shader->effect);
            shader->effect->lpVtbl->End(shader->effect);
            rasterizer_set_texture_bitmap_data(0, sweep_bitmap);

            D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, 2);
            D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, 2);
            D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, 1);
            D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, 1);
            D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 2);

            D3DDevice_SetRenderState_CullMode(global_d3d_device, 6);
            D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
            D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
            D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 1);
            D3DDevice_SetRenderState_DestBlend(global_d3d_device, 6);
            D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0);
            D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
            D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);

            D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, vsh_constants__screenproj, 5, 3ULL << 59);

            SetTextureStageStateSmart(0, D3DTSS_COLOROP,   4);
            SetTextureStageStateSmart(0, D3DTSS_COLORARG1, 2);
            SetTextureStageStateSmart(0, D3DTSS_COLORARG2, 0);
            SetTextureStageStateSmart(0, D3DTSS_ALPHAOP,   2);
            SetTextureStageStateSmart(0, D3DTSS_ALPHAARG1, 0);
            SetTextureStageStateSmart(1, D3DTSS_COLOROP,   1);
            SetTextureStageStateSmart(1, D3DTSS_ALPHAOP,   1);

            /* Pass 1: sweep/fade over the whole [-1.015625, 1.046875] target rect. Its U ramps with
             * theta so the radar "sweep" wipes across; skipped once theta has swept past 2.75. */
            if ( theta <= 2.75 )
            {
                float sweep_hi = ((theta * 0.5f) + 0.5f);
                float sweep_lo = (0.5f - (theta * 0.5f));
                /* Vertex stream: {x,y,z, packed ARGB dword in the float slot, u,v}; sweep tint = 0xFF74B9FF. */
                float sweep[24];
                sweep[0]  = -1.015625f; sweep[1]  =  1.046875f; sweep[2]  = 0.0f;
                *(int *)&sweep[3]  = -9127425; sweep[4]  = sweep_hi; sweep[5]  = sweep_lo;
                sweep[6]  =  1.046875f; sweep[7]  =  1.046875f; sweep[8]  = 0.0f;
                *(int *)&sweep[9]  = -9127425; sweep[10] = sweep_lo; sweep[11] = sweep_lo;
                sweep[12] =  1.046875f; sweep[13] = -1.015625f; sweep[14] = 0.0f;
                *(int *)&sweep[15] = -9127425; sweep[16] = sweep_lo; sweep[17] = sweep_hi;
                sweep[18] = -1.015625f; sweep[19] = -1.015625f; sweep[20] = 0.0f;
                *(int *)&sweep[21] = -9127425; sweep[22] = sweep_hi; sweep[23] = sweep_hi;
                D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4, sweep, 0x18);
            }

            /* Pass 2: multiply in the range-ring sheet (bitmap 8) via the effect. */
            D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 0);
            rasterizer_set_texture_bitmap_data_for_effect(0, ring_bitmap, shader);
            shader->effect->lpVtbl->Begin(shader->effect, passes, 3);
            shader->effect->lpVtbl->BeginPass(shader->effect, 0);
            D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
            D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 0);
            D3DDevice_SetRenderState_DestBlend(global_d3d_device, 6);

            pixel_constant[0] = 1.0f; pixel_constant[1] = 1.0f; pixel_constant[2] = 1.0f;
            pixel_constant[3] = 1.0f; pixel_constant[4] = 1.0f; pixel_constant[5] = 0.0f;
            D3DDevice_SetPixelShaderConstantFN(global_d3d_device, 0, pixel_constant, 2, 1ULL << 63);

            /* Vertex stream as above; ring tint = 0xFF66CC66. */
            float ring[24];
            ring[0]  = -1.015625f; ring[1]  =  1.046875f; ring[2]  = 0.0f;
            *(int *)&ring[3]  = -10040218; ring[4]  = 1.0f; ring[5]  = 0.0f;
            ring[6]  =  1.046875f; ring[7]  =  1.046875f; ring[8]  = 0.0f;
            *(int *)&ring[9]  = -10040218; ring[10] = 0.0f; ring[11] = 0.0f;
            ring[12] =  1.046875f; ring[13] = -1.015625f; ring[14] = 0.0f;
            *(int *)&ring[15] = -10040218; ring[16] = 0.0f; ring[17] = 1.0f;
            ring[18] = -1.015625f; ring[19] = -1.015625f; ring[20] = 0.0f;
            *(int *)&ring[21] = -10040218; ring[22] = 1.0f; ring[23] = 1.0f;
            D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4, ring, 0x18);

            shader->effect->lpVtbl->EndPass(shader->effect);
            shader->effect->lpVtbl->End(shader->effect);

            /* Pass 3: draw the finished radar disc onto the screen at center_point. */
            rasterizer_set_target(global_window_parameters.rasterizer_target, 0, 0, 0, 1);

            D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, 2);
            D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, 2);
            D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, 1);
            D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, 1);
            D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 0);

            D3DDevice_SetRenderState_CullMode(global_d3d_device, 6);
            D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
            D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
            D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 1);
            D3DDevice_SetRenderState_DestBlend(global_d3d_device, 1);
            D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0);
            D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
            D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);

            /* Screen→NDC projection built from the current viewport's width and height. */
            float viewport_width  = (float)(int16_t)(global_window_parameters.camera.viewport_bounds.n[3]
                                                   - global_window_parameters.camera.viewport_bounds.n[1]);
            float viewport_height = (float)(int16_t)(global_window_parameters.camera.viewport_bounds.n[2]
                                                   - global_window_parameters.camera.viewport_bounds.n[0]);
            float screen_proj[20];
            screen_proj[0]  = (1.0f / viewport_width)  *  2.0f;
            screen_proj[1]  = 0.0f;
            screen_proj[2]  = 0.0f;
            screen_proj[3]  = -1.0f - (1.0f / viewport_width);
            screen_proj[4]  = 0.0f;
            screen_proj[5]  = (1.0f / viewport_height) * -2.0f;
            screen_proj[6]  = 0.0f;
            screen_proj[7]  = (1.0f / viewport_height) +  1.0f;
            screen_proj[8]  = 0.0f;
            screen_proj[9]  = 0.0f;
            screen_proj[10] = 0.0f;
            screen_proj[11] = 0.5f;
            screen_proj[12] = 0.0f;
            screen_proj[13] = 0.0f;
            screen_proj[14] = 0.0f;
            screen_proj[15] = 1.0f;
            screen_proj[16] = 1.0f;
            screen_proj[17] = 1.0f;
            screen_proj[18] = 0.0f;
            screen_proj[19] = 1.0f;
            D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, screen_proj, 5, 3ULL << 59);

            /* Radar disc half-size: HUD scale × 84px (single) or 64px (split-screen). */
            float base_size = (local_player_count() <= 1) ? 84.0f : 64.0f;
            float scale = hud_globals_get_scale(local_player_count() > 1);
            float half = (scale * base_size);

            /* Vertex stream as above; disc tint = opaque white 0xFFFFFFFF. */
            float disc[24];
            disc[0]  = center_point->n[0] - half; disc[1]  = center_point->n[1] - half; disc[2]  = 0.0f;
            *(int *)&disc[3]  = -1; disc[4]  = 0.0f; disc[5]  = 0.0f;
            disc[6]  = center_point->n[0] + half; disc[7]  = center_point->n[1] - half; disc[8]  = 0.0f;
            *(int *)&disc[9]  = -1; disc[10] = 1.0f; disc[11] = 0.0f;
            disc[12] = center_point->n[0] + half; disc[13] = center_point->n[1] + half; disc[14] = 0.0f;
            *(int *)&disc[15] = -1; disc[16] = 1.0f; disc[17] = 1.0f;
            disc[18] = center_point->n[0] - half; disc[19] = center_point->n[1] + half; disc[20] = 0.0f;
            *(int *)&disc[21] = -1; disc[22] = 0.0f; disc[23] = 1.0f;

            rasterizer_set_target_as_texture_for_effect(0, 5, 0, shader);
            shader->effect->lpVtbl->Begin(shader->effect, passes, 3);
            shader->effect->lpVtbl->BeginPass(shader->effect, 0);
            pixel_constant[0] = 1.0f; pixel_constant[1] = 1.0f; pixel_constant[2] = 1.0f;
            pixel_constant[3] = 1.0f; pixel_constant[4] = 1.0f; pixel_constant[5] = 0.0f;
            D3DDevice_SetPixelShaderConstantFN(global_d3d_device, 0, pixel_constant, 2, 1ULL << 63);
            D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4, disc, 0x18);
            shader->effect->lpVtbl->EndPass(shader->effect);
            shader->effect->lpVtbl->End(shader->effect);
            return;
        }
        draw_hud_motion_sensor = rasterizer_debug_options.draw_hud_motion_sensor;
    }

    if ( draw_hud_motion_sensor && rasterizer_motion_sensor_begin_said_to_draw )
        rasterizer_set_target(global_window_parameters.rasterizer_target, 0, 0, 0, 1);
}
