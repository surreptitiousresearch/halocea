/* _rasterizer_psuedo_dynamic_screen_quad_draw_fx @0x837A8E90 — bind up to 3 textures from
 * parameters->map[], then run the effect's technique passes, drawing a 4-vertex triangle-fan quad with
 * the given pixel shader constants on each pass. Combines each D3DX effect call's high bit (their HRESULT
 * sign bit, per the "convert to 1 on success / 0 on failure" idiom already used elsewhere in this
 * project) so the overall return value is 1 only if Begin/every BeginPass+EndPass/End all succeeded.
 *
 * DEVIATION: the decompiler drops the constant PendingMask0 argument to D3DDevice_SetPixelShaderConstantFN
 * (rendered as an uninitialized local); disasm shows it's the literal (unsigned __int64)3 << 62. */

#include <stdint.h>
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"


#include "headers/bitmap_data.h"
extern int rasterizer_set_texture_bitmap_data_for_effect(int16_t stage, bitmap_data *bitmap, rasterizer_dx9_shader *dxeffect_shader);
extern void D3DDevice_SetPixelShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
    const float *pConstantData, unsigned int Vector4fCount, unsigned __int64 PendingMask0);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
    const void *vertex_data, unsigned int stride);

uint8_t _rasterizer_psuedo_dynamic_screen_quad_draw_fx(
    const rasterizer_dynamic_screen_geometry_parameters *parameters, dynamic_screen_vertex *verts,
    rasterizer_dx9_shader *dxeffect_shader, const float *psh_constants)
{
    for ( int16_t stage = 0; stage < 3; ++stage )
    {
        if ( !parameters->map[stage] )
            break;
        rasterizer_set_texture_bitmap_data_for_effect(stage, parameters->map[stage], dxeffect_shader);
    }

    unsigned int pass_count = 0;
    unsigned int ok = (((unsigned int)dxeffect_shader->effect->lpVtbl->Begin(dxeffect_shader->effect, &pass_count, 3u) >> 31) - 1) & 1;

    for ( unsigned int pass = 0; pass < pass_count; ++pass )
    {
        ok = (((unsigned int)dxeffect_shader->effect->lpVtbl->BeginPass(dxeffect_shader->effect, pass) >> 31) - 1) & ok;

        D3DDevice_SetPixelShaderConstantFN(global_d3d_device, 0, psh_constants, 6u, (unsigned __int64)3 << 62);
        D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4u, verts, 0x18u);

        ok = (((unsigned int)dxeffect_shader->effect->lpVtbl->EndPass(dxeffect_shader->effect) >> 31) - 1) & ok;
    }

    return (((unsigned int)dxeffect_shader->effect->lpVtbl->End(dxeffect_shader->effect) >> 31) - 1) & ok;
}
