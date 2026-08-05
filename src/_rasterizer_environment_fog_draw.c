/* _rasterizer_environment_fog_draw @0x8378FD28 — draw the planar+atmospheric fog pass over one environment
 * surface batch. Runs only when fog is globally enabled and not suppressed. Selects the fog effect shader
 * (slot 112), binds the structure vertex declaration/shader for the batch's vertex format and the shader's
 * vertex permutation (+11 fog variant), uploads four vector parameters describing the atmospheric and planar
 * fog densities and colors, then draws the dynamic triangles once per effect pass.
 *
 * The four uploaded vectors are:
 *   c0 atmospheric: (max_density, max_density, max_density, eye_blend * max_density)
 *   c1 planar:      ((1-eye_density)*planar_max ×3, planar_max * eye_density)
 *   c2 atmospheric color: (r, g, b, eye_blend)
 *   c3 planar color:      (r, g, b, 1 - eye_blend)
 * where eye_blend is the atmospheric/planar mix factor rasterizer_fog_eye_blend_factor and eye_density is planar_eye_density.
 *
 * DEVIATION: the canonical 6-arg draw-callback signature is used (the thunk establishes it); this pass does
 * not index the shader struct, so no shader_environment retyping is needed. rasterizer_fog_eye_blend_factor retains its database
 * symbol name (no source-level name is recorded for the fog eye-blend factor). */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/vertex_buffer.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/blam_data_globals.h"

extern float rasterizer_fog_eye_blend_factor;   /* atmospheric/planar fog eye-blend factor */

extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern int16_t shader_get_vertex_shader_permutation(const shader *shader);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void rasterizer_draw_dynamic_triangles_static_vertices(int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);

void _rasterizer_environment_fog_draw(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer)
{
    if (rasterizer_debug_options.drawing_mode || !global_fog_enabled || rasterizer_environment_fog_off)
        return;

    rasterizer_dx9_shader *effect_shader = rasterizer_shader_select(_dxshader_environment_fog);
    if (!effect_shader || !effect_shader->effect)
        return;

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(vertex_buffer->type));
    int16_t vertex_shader_permutation = shader_get_vertex_shader_permutation(shader);
    D3DDevice_SetVertexShader(global_d3d_device,
                              rasterizer_dx9_shaders_vshader9_get(vertex_shader_permutation + _vs_environment_fog));

    ID3DXEffect *effect = effect_shader->effect;
    unsigned int *constants = effect_shader->constants;

    float atmospheric_density[4];
    atmospheric_density[0] = global_window_parameters.fog.atmospheric_maximum_density;
    atmospheric_density[1] = global_window_parameters.fog.atmospheric_maximum_density;
    atmospheric_density[2] = global_window_parameters.fog.atmospheric_maximum_density;
    atmospheric_density[3] = rasterizer_fog_eye_blend_factor * global_window_parameters.fog.atmospheric_maximum_density;

    float planar_density[4];
    planar_density[3] = global_window_parameters.fog.planar_maximum_density * planar_eye_density;
    planar_density[0] = (1.0f - planar_eye_density) * global_window_parameters.fog.planar_maximum_density;
    planar_density[1] = planar_density[0];
    planar_density[2] = planar_density[0];

    float atmospheric_color[4];
    atmospheric_color[0] = global_window_parameters.fog.atmospheric_color.__s1.red;
    atmospheric_color[1] = global_window_parameters.fog.atmospheric_color.__s1.green;
    atmospheric_color[2] = global_window_parameters.fog.atmospheric_color.__s1.blue;
    atmospheric_color[3] = rasterizer_fog_eye_blend_factor;

    float planar_color[4];
    planar_color[0] = global_window_parameters.fog.planar_color.__s1.red;
    planar_color[1] = global_window_parameters.fog.planar_color.__s1.green;
    planar_color[2] = global_window_parameters.fog.planar_color.__s1.blue;
    planar_color[3] = 1.0f - rasterizer_fog_eye_blend_factor;

    effect->lpVtbl->SetVector(effect, constants[0], (const D3DXVECTOR4 *)atmospheric_density);
    effect_shader->effect->lpVtbl->SetVector(effect_shader->effect, effect_shader->constants[1],
                                     (const D3DXVECTOR4 *)planar_density);
    effect_shader->effect->lpVtbl->SetVector(effect_shader->effect, effect_shader->constants[2],
                                     (const D3DXVECTOR4 *)atmospheric_color);
    effect_shader->effect->lpVtbl->SetVector(effect_shader->effect, effect_shader->constants[3],
                                     (const D3DXVECTOR4 *)planar_color);

    unsigned int pass_count[4];
    effect_shader->effect->lpVtbl->Begin(effect_shader->effect, pass_count, 3);
    for (unsigned int i = 0; i < pass_count[0]; ++i)
    {
        effect_shader->effect->lpVtbl->BeginPass(effect_shader->effect, i);
        rasterizer_draw_dynamic_triangles_static_vertices(dynamic_triangle_buffer_index, first_triangle_index,
                                                          triangle_count, vertex_buffer);
        effect_shader->effect->lpVtbl->EndPass(effect_shader->effect);
    }
    effect_shader->effect->lpVtbl->End(effect_shader->effect);
}
