/* _rasterizer_hud_motion_sensor_blip_begin @0x837AA020 — the implementation behind the
 * rasterizer_hud_motion_sensor_blip_begin thunk: prepare GPU state for drawing the motion-sensor (radar) blips.
 * It ensures the two interface bitmap groups (12 = blip sprites, 13 = a second sheet) are resident, clears the
 * "began drawing" flag, and — only when render targets are enabled, radar debug drawing is on, and both bitmaps
 * have a valid hardware format — selects the blip effect shader (122), retargets to render target 5 (cleared),
 * sets additive-blend / no-depth 2D render state, binds the blip vertex declaration/shader, uploads the
 * identity view matrix (5 vec4 at register 0xD), binds the blip bitmap into the effect, begins the effect and
 * its first pass, sets clamp addressing + linear filtering on sampler 0, and uploads an all-ones pixel-shader
 * constant. Sibling of rasterizer_hud_motion_sensor_blip_draw / _rasterizer_detail_objects_begin.
 *
 * DEVIATION: the decompiler rendered the inline sampler-state register pokes as raw
 * global_d3d_device->m_Constants.Fetch[0].Texture writes with __ROL4__ masks; disasm (0x837AA284-2FC) shows the
 * standard inline helpers — AddressU=2, AddressV=2 (rlwimi of r31=1), MagFilter=1, MinFilter=1, and a dword[3]
 * clear = SeparateZFilterEnable=0 — matching the _rasterizer_decals_begin sibling. The vertex-constant count/mask
 * (5, 3ULL<<59) and the pixel-constant count (2) come from disasm; the PendingMask args are GPU pending-register
 * bitmasks (boundary values). The two tag_get_name() calls' results are discarded (debug/asset validation). */

#include <stdint.h>
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/bitmap_data.h"
#include "headers/d3d_boundary.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/interface_tag_index.h"
#include "headers/_D3DTEXTUREFILTERTYPE.h"
#include "headers/_D3DBLENDOP.h"
#include "headers/_D3DCULL.h"
#include "headers/_D3DBLEND.h"
#include "headers/_D3DTEXTUREADDRESS.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
extern void D3DDevice_SetPixelShaderConstantFN(D3DDevice *device, unsigned int StartRegister, const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask1);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);


extern int interface_get_tag_index(int16_t interface_tag_index);
extern bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index);
extern char *tag_get_name(int tag_index);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern void rasterizer_set_target(int16_t target, int16_t mipmap_index, uint32_t background_color, uint8_t clear, uint8_t zbuffer);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern int rasterizer_set_texture_bitmap_data_for_effect(int16_t stage, bitmap_data *bitmap, rasterizer_dx9_shader *dxeffect_shader);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
        unsigned int value);

void _rasterizer_hud_motion_sensor_blip_begin(void)
{
    bitmap_data *blip_bitmap = bitmap_group_try_and_get_bitmap(interface_get_tag_index(_interface_bitmap_motion_blip), 0);
    bitmap_data *sheet_bitmap = bitmap_group_try_and_get_bitmap(interface_get_tag_index(_interface_bitmap_iface_map1), 0);
    tag_get_name(interface_get_tag_index(_interface_bitmap_motion_blip));
    tag_get_name(interface_get_tag_index(_interface_bitmap_iface_map1));

    rasterizer_motion_sensor_begin_said_to_draw = 0;

    if ( rasterizer_globals.render_targets_disabled
      || !rasterizer_debug_options.draw_hud_motion_sensor
      || !_texture_cache_bitmap_get_hardware_format(blip_bitmap, 0, 1)
      || !_texture_cache_bitmap_get_hardware_format(sheet_bitmap, 0, 1) )
        return;

    float pixel_constant[8];
    for ( int i = 0; i < 8; ++i )
        pixel_constant[i] = 0.0f;

    rasterizer_dx9_shader *shader = rasterizer_shader_select(_dxshader_dyn_geom);
    rasterizer_motion_sensor_begin_said_to_draw = 1;

    rasterizer_set_target(5, 0, 0, 1, 0);
    D3DDevice_SetRenderState_CullMode(global_d3d_device, D3DCULL_CCW);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_SrcBlend(global_d3d_device, D3DBLEND_ONE);
    D3DDevice_SetRenderState_DestBlend(global_d3d_device, D3DBLEND_ONE);
    D3DDevice_SetRenderState_BlendOp(global_d3d_device, D3DBLENDOP_ADD);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_screen));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_screen));

    /* identity view matrix (rows) + a (1,1,0,1) parameter row — 5 vec4 at register 0xD */
    float view_constants[20] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f,
    };
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, view_constants, 5, 3ULL << 59);

    rasterizer_set_texture_bitmap_data_for_effect(0, blip_bitmap, shader);

    unsigned int passes[4];
    shader->effect->lpVtbl->Begin(shader->effect, passes, 3);
    shader->effect->lpVtbl->BeginPass(shader->effect, 0);

    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 0);

    pixel_constant[0] = 1.0f;
    pixel_constant[1] = 1.0f;
    pixel_constant[2] = 1.0f;
    pixel_constant[3] = 1.0f;
    pixel_constant[4] = 1.0f;
    pixel_constant[5] = 1.0f;
    D3DDevice_SetPixelShaderConstantFN(global_d3d_device, 0, pixel_constant, 2, 1ULL << 63);
}
