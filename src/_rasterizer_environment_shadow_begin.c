#include <stdint.h>
#include <string.h>
#include "headers/d3d_boundary.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_rgb_color.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_engine_globals.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_target.h"
#include "headers/blam_data_globals.h"

extern void rasterizer_set_target(int16_t target, int16_t mipmap_index, uint32_t background_color, uint8_t clear, uint8_t zbuffer);
extern void rasterizer_set_stencil_mode(int16_t stencil_mode);

extern int            rasterizer_shadow_bounding_radius;       /* float bounding_radius stored as int bits */

/* DEVIATION: DB proto uses float object_bounding_radius; decompiler widens to double via FPR-shadow */
unsigned __int8 _rasterizer_environment_shadow_begin(
        int                   object_index,
        const real_matrix4x3 *shadow_matrix,
        const real_rgb_color *shadow_color,
        float                 object_bounding_radius,
        float                *shadow_volume_bounding_radius)
{
    if (global_window_parameters.rasterizer_target != _rasterizer_target_render_primary)
        return 1;

    if (!rasterizer_globals.render_targets_disabled
     && rasterizer_debug_options.draw_environment_shadows)
    {
        D3DDevice_SetRenderState_CullMode(global_d3d_device, 6u);
        D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7u);
        D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
        D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 1u);
        D3DDevice_SetRenderState_AlphaRef(global_d3d_device, 0x7Fu);
        D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);

        float inv_radius = 1.0f / object_bounding_radius;
        float shadow_shader_constants[22];

        shadow_shader_constants[0]  = shadow_matrix->n[0][0] * inv_radius;
        shadow_shader_constants[1]  = shadow_matrix->n[0][1] * inv_radius;
        shadow_shader_constants[2]  = shadow_matrix->n[0][2] * inv_radius;
        shadow_shader_constants[3]  = -(shadow_matrix->n[0][0] * shadow_matrix->n[3][0]
                  + shadow_matrix->n[0][1] * shadow_matrix->n[3][1]
                  + shadow_matrix->n[0][2] * shadow_matrix->n[3][2]) * inv_radius;
        shadow_shader_constants[4]  = shadow_matrix->n[1][0] * inv_radius;
        shadow_shader_constants[5]  = shadow_matrix->n[1][1] * inv_radius;
        shadow_shader_constants[6]  = shadow_matrix->n[1][2] * inv_radius;
        shadow_shader_constants[7]  = -(shadow_matrix->n[1][0] * shadow_matrix->n[3][0]
                  + shadow_matrix->n[1][1] * shadow_matrix->n[3][1]
                  + shadow_matrix->n[1][2] * shadow_matrix->n[3][2]) * inv_radius;
        shadow_shader_constants[8]  = 0.0f;
        shadow_shader_constants[9]  = 0.0f;
        shadow_shader_constants[10] = 0.0f;
        shadow_shader_constants[11] = 0.5f;
        shadow_shader_constants[12] = 0.0f;
        shadow_shader_constants[13] = 0.0f;
        shadow_shader_constants[14] = 0.0f;
        shadow_shader_constants[15] = 1.0f;
        shadow_shader_constants[16] = 0.0f;
        shadow_shader_constants[17] = 0.0f;
        shadow_shader_constants[18] = 0.0f;
        shadow_shader_constants[19] = 0.0f;
        /* shadow_shader_constants[20] and shadow_shader_constants[21] are not explicitly assigned (stack undefined) */

        D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xDu, shadow_shader_constants, 0x18000000u, 0);

        rasterizer_set_target(3, 0,
                rasterizer_debug_options.shadow_debug_enabled ? 0x88888888u : 0u,
                1u, 0);
        rasterizer_set_stencil_mode(0);
        memcpy(&environment_shadow_projection_matrix, shadow_matrix, 0x34u);

        local_shadow_color.n[0] = shadow_color->n[0];
        local_shadow_color.n[1] = shadow_color->n[1];
        local_shadow_color.n[2] = shadow_color->n[2];

        /* store float bounding_radius bits into rasterizer_shadow_bounding_radius */
        int tmp_radius_bits;
        *(float *)&tmp_radius_bits = object_bounding_radius;
        rasterizer_shadow_bounding_radius = tmp_radius_bits;

        if (shadow_volume_bounding_radius)
            *shadow_volume_bounding_radius = object_bounding_radius;

        local_parameters_0 = NULL;
        shadow_setup        = 0;
        shadow_used         = 0;
        shadow_restored     = 0;
        return 1;
    }

    if (shadow_volume_bounding_radius)
        *shadow_volume_bounding_radius = 0.0f;
    return 1;
}
