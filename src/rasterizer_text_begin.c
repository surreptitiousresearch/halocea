/* rasterizer_text_begin @0x83814A40 — open a dynamic screen-geometry text pass. Selects the text shader
 * (slot 122), pushes the render-state needed to draw alpha-blended, alpha-tested glyphs with depth testing
 * off, binds the screen-geometry vertex declaration/shader, uploads the glyph bitmap's texel scale as a
 * vertex-shader constant, binds the glyph texture into the effect, begins the effect + first pass, sets the
 * texture-stage filtering/wrap state from the parameters, and uploads a white pixel-shader tint constant.
 * Only runs when dynamic screen geometry drawing is enabled and the target is the main framebuffer.
 *
 * DEVIATION: the decompiler rendered the local constant scratch buffer zeroing as a pointer-increment loop;
 * restored to an array clear. The trailing Vector4fCount / PendingMask arguments to the Xbox *ShaderConstantFN
 * fast-path intrinsics are register-packed values the decompiler could not fully resolve (it showed an
 * uninitialized mask for the vertex upload); reproduced from the disassembly. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/bitmap_data.h"
#include "headers/rasterizer_target.h"
#include "headers/_D3DCULL.h"
#include "headers/_D3DFILLMODE.h"
#include "headers/_D3DSAMPLERSTATETYPE.h"
#include "headers/_D3DTEXTUREADDRESS.h"
#include "headers/_D3DTEXTUREFILTERTYPE.h"
#include "headers/blam_data_globals.h"


#include "headers/bitmap_data.h"
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern void rasterizer_text_handle_mode_change(void);
extern void rasterizer_set_framebuffer_blend_function(int16_t framebuffer_blend_function);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern int rasterizer_set_texture_bitmap_data_for_effect(int16_t stage, bitmap_data *bitmap, rasterizer_dx9_shader *dxeffect_shader);
extern void SetTextureStageStateSmart(unsigned int stage, _D3DTEXTURESTAGESTATETYPE State, unsigned int Value);
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int mode);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_AlphaRef(D3DDevice *device, unsigned int ref);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_FillMode(D3DDevice *device, unsigned int fill_mode);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
                                                const float *pConstantData, unsigned int Vector4fCount,
                                                uint64_t PendingMask0);
extern void D3DDevice_SetPixelShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
                                               const float *pConstantData, unsigned int Vector4fCount,
                                               uint64_t PendingMask1);

void rasterizer_text_begin(const rasterizer_dynamic_screen_geometry_parameters *parameters)
{
    if (!rasterizer_debug_options.draw_dynamic_screen_geometry || global_window_parameters.rasterizer_target != _rasterizer_target_render_primary)
        return;

    rasterizer_dx9_shader *shader = rasterizer_shader_select(_dxshader_dyn_geom);

    unsigned int pass_count[4];
    float constants[20];
    for (int i = 0; i < 8; ++i)
        constants[i] = 0.0f;

    rasterizer_text_handle_mode_change();
    rasterizer_set_framebuffer_blend_function(parameters->framebuffer_blend_function);
    D3DDevice_SetRenderState_CullMode(global_d3d_device, D3DCULL_CCW);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_AlphaRef(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);
    if (rasterizer_debug_options.wireframe_enabled)
        D3DDevice_SetRenderState_FillMode(global_d3d_device, D3DFILL_SOLID);

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_screen));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_screen));

    local_matrix_projection[4][0] = parameters->map_texture_scale[0].n[0];
    local_matrix_projection[4][1] = parameters->map_texture_scale[0].n[1];
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, local_matrix_projection[0], 0x18000000, 0);

    rasterizer_set_texture_bitmap_data_for_effect(0, parameters->map[0], shader);
    ID3DXEffect_Begin(shader->effect, pass_count, 3);
    ID3DXEffect_BeginPass(shader->effect, 0);

    /* DEVIATION: the old inline comments guessed PC-D3D9 D3DTSS_* selectors; the 360 second
     * argument is the _D3DSAMPLERSTATETYPE register selector (stride-4 numbering, DB-verified). */
    SetTextureStageStateSmart(0, D3DSAMP_ADDRESSU, parameters->map_wrapped[0] != 0 ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP);
    SetTextureStageStateSmart(0, D3DSAMP_ADDRESSV, parameters->map_wrapped[0] != 0 ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP);
    SetTextureStageStateSmart(0, D3DSAMP_MAGFILTER, parameters->point_sampled ? D3DTEXF_POINT : D3DTEXF_LINEAR);
    SetTextureStageStateSmart(0, D3DSAMP_MINFILTER, parameters->point_sampled ? D3DTEXF_POINT : D3DTEXF_LINEAR);
    SetTextureStageStateSmart(0, D3DSAMP_MIPFILTER, parameters->point_sampled ? D3DTEXF_POINT : D3DTEXF_LINEAR);

    constants[0] = 1.0f;
    constants[1] = 1.0f;
    constants[2] = 1.0f;
    constants[3] = 1.0f;
    constants[4] = 1.0f;
    constants[5] = 1.0f;
    D3DDevice_SetPixelShaderConstantFN(global_d3d_device, 0, constants, 2, 0x8000000000000000ULL);
}
