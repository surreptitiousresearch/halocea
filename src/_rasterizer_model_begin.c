/* _rasterizer_model_begin @0x8378B060 — per-frame, per-model rasterizer setup called before drawing a model's
 * geometry. Configures the four texture sampler stages used by model shaders (wrap addressing reset + point
 * min/mag/mip filtering for stages 0-2, clamp addressing for stage 3), then — only while model drawing is
 * enabled for debugging — latches `parameters`/`do_not_change_z_stencil_states` into the module-level
 * "current model" context consumed by `_rasterizer_model_draw`/`_rasterizer_model_end`, resolves the active
 * camouflage / model-effect type (pushing skinning + lighting constants unless the model is skipping that
 * because it's itself a screen effect, effect type 2), and finally recomputes the global atmospheric fog
 * density blend for this model's centroid (a depth/distance-based fade combining the fog plane's cutoff
 * with the camera's own position relative to it).
 *
 * DEVIATION: the DB's decompiler output for this function carries the "local variable allocation has failed"
 * warning — the whole first half (four sampler-stage setups) is displayed as raw GPU fetch-constant register
 * pokes (`m_Constants.Fetch[N].Texture.dword[0/3]` + `m_Pending.m_Mask` dirty-bit ORs) with no recoverable
 * call arguments. Reconstructed directly from `disasm_range` and matched against the identical raw-poke
 * pattern already resolved in sibling `_rasterizer_environment_*_begin`/`_rasterizer_detail_objects_begin`
 * functions: each poke is the compiled body of a genuine Xbox 360 D3D9 XDK `D3DDevice_SetSamplerState_*_Inline`
 * call (address-mode fields folding to a pure register clear when the passed value is 0), immediately
 * paired with the real (non-inlined) `_MagFilter`/`_MinFilter` calls visible as ordinary `bl`s in the same
 * disasm — so this is written as the five/six equivalent high-level calls per stage rather than the raw
 * bit-twiddling, exactly matching how those sibling functions were already restored. */

#include <stdint.h>
#include "headers/d3d_boundary.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_model_begin_parameters.h"
#include "headers/rasterizer_target.h"
#include "headers/rasterizer_geometry_flags.h"
#include "headers/rasterizer_stencil_mode.h"
#include "headers/render_model_effect_type.h"
#include "headers/blam_data_globals.h"


extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_AddressW_Inline(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MipFilter_Inline(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);

extern void rasterizer_set_stencil_mode(int16_t stencil_mode);
extern void rasterizer_set_frustum_z(float z_near, float z_far);
extern void rasterizer_set_model_skinning(const render_skinning *skinning, uint8_t uploadconstants);
extern void rasterizer_set_model_lighting(const render_lighting *lighting);
extern uint8_t rasterizer_environment_fog_screen_model_begin(const rasterizer_model_begin_parameters *parameters);

/* module-level "current model" context shared with _rasterizer_model_draw/_rasterizer_model_end */


static float clamp01(float value)
{
    if (value < 0.0f)
        return 0.0f;
    if (value > 1.0f)
        return 1.0f;
    return value;
}

void _rasterizer_model_begin(const rasterizer_model_begin_parameters *parameters,
                             uint8_t do_not_change_z_stencil_states)
{
    /* Stage 0: wrap addressing, point min/mag/mip filtering */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, 0);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, 0);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, 1);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, 1);
    D3DDevice_SetSamplerState_MipFilter_Inline(global_d3d_device, 0, 1);
    /* Stage 1 */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 1, 0);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 1, 0);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 1, 1);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 1, 1);
    D3DDevice_SetSamplerState_MipFilter_Inline(global_d3d_device, 1, 1);
    /* Stage 2 */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 2, 0);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 2, 0);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 2, 1);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 2, 1);
    D3DDevice_SetSamplerState_MipFilter_Inline(global_d3d_device, 2, 1);
    /* Stage 3: clamp addressing (all three axes — cube/volume-capable stage) */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 3, 1);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 3, 1);
    D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, 3, 1);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 3, 1);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 3, 1);
    D3DDevice_SetSamplerState_MipFilter_Inline(global_d3d_device, 3, 1);

    if (!rasterizer_debug_options.draw_models)
        return;

    if ((parameters->geometry_flags & (1u << _rasterizer_geometry_first_person_bit)) != 0 && !do_not_change_z_stencil_states)
    {
        rasterizer_set_stencil_mode(_rasterizer_stencil_mode_write);
        rasterizer_set_frustum_z(rasterizer_globals.z_near_first_person, rasterizer_globals.z_far_first_person);
    }

    local_parameters = parameters;
    local_parameters_queued_flag = 0;
    local_do_not_change_z_stencil_states = do_not_change_z_stencil_states;

    int model_effect_type;
    if (rasterizer_debug_options.active_camouflage_enabled
        && global_window_parameters.rasterizer_target == _rasterizer_target_render_primary
        && parameters->effect.type == _render_model_effect_type_active_camouflage
        && parameters->effect.intensity > 0.0f)
    {
        model_effect_type = _render_model_effect_type_active_camouflage;
    }
    else
    {
        model_effect_type = (unsigned __int16)parameters->effect.type;
        if (model_effect_type != _render_model_effect_type_transparent_zbuffered)
        {
            /* uploadconstants: skip re-uploading skinning constants for rigid (single-node) models */
            rasterizer_set_model_skinning(&parameters->skinning,
                                          (~parameters->geometry_flags >> 8) & 1);
            rasterizer_set_model_lighting(&parameters->lighting);
            model_effect_type = _render_model_effect_type_none;
        }
    }
    local_model_effect_type = model_effect_type;

    const render_fog *fog = &global_window_parameters.fog;
    const render_camera *camera = &global_window_parameters.camera;

    /* camera's own signed distance to the fog plane (dot(normal, camera_position) - distance); reused below
     * for the atmospheric density blend, matching the original's redundant reload-and-recompute */
    float camera_plane_distance = camera->position.n[0] * fog->plane.n.n[0]
                                 + camera->position.n[2] * fog->plane.n.n[2]
                                 + camera->position.n[1] * fog->plane.n.n[1]
                                 - fog->plane.d;

    local_planar_fog_flag = fog->planar_mode
                          && (parameters->geometry_flags & (1u << _rasterizer_geometry_no_fog_bit)) == 0
                          && ((parameters->geometry_flags & (1u << _rasterizer_geometry_atmospheric_fog_but_no_planar_fog_bit)) == 0 || camera_plane_distance < 0.0f);

    /* Simplified from the original's "goto"-based short-circuit: skip the fog-screen test entirely under a
     * sky, otherwise the flag follows whether this model is behind a fog screen surface. */
    unsigned char environment_fog_screen_flag = 0;
    if (!local_sky_flag)
        environment_fog_screen_flag = rasterizer_environment_fog_screen_model_begin(parameters) != 0;
    local_environment_fog_screen_flag = environment_fog_screen_flag;

    /* FAITHFUL QUIRK: reproduced exactly as compiled — the precise game-design intent behind each blend term
     * isn't fully certain, but every load/field here was cross-checked against render_fog/render_camera's
     * database offsets. camera_plane_distance is the camera's own signed distance to the fog plane;
     * centroid_depth_fade/centroid_distance_fade fade the model out near the fog plane's cutoff depth and
     * the camera's maximum fog distance respectively; camera_plane_fade further fades based on which side of
     * the fog plane the camera itself sits on. */
    float oo_planar_maximum_depth = 1.0f / fog->planar_maximum_depth;
    float oo_planar_maximum_distance = 1.0f / fog->planar_maximum_distance;

    float centroid_dot_normal = fog->plane.n.n[0] * parameters->centroid.n[0]
                              * -oo_planar_maximum_depth
                              + (-(fog->plane.n.n[1] * oo_planar_maximum_depth) * parameters->centroid.n[1]
                              + -(fog->plane.n.n[2] * oo_planar_maximum_depth) * parameters->centroid.n[2]);
    float centroid_depth_fade = clamp01(1.0f
        - (fog->plane.d * oo_planar_maximum_depth + centroid_dot_normal));

    float camera_dot_forward = camera->position.n[0] * camera->forward.n[0]
                             + camera->position.n[2] * camera->forward.n[2]
                             + camera->position.n[1] * camera->forward.n[1];
    float centroid_distance_fade = clamp01(1.0f
        - (camera->forward.n[2] * oo_planar_maximum_distance * parameters->centroid.n[2]
         + (camera->forward.n[1] * oo_planar_maximum_distance * parameters->centroid.n[1]
         + (camera->forward.n[0] * oo_planar_maximum_distance * parameters->centroid.n[0]
         + -(camera_dot_forward * oo_planar_maximum_distance)))));

    float combined_fade = centroid_distance_fade + centroid_depth_fade;
    if (combined_fade > 1.0f)
        combined_fade = 1.0f;
    float inverse_combined_fade = 1.0f - combined_fade;

    float camera_plane_fade = clamp01(-(camera_plane_distance * oo_planar_maximum_depth));

    float distance_fade_sq_delta = (1.0f - centroid_distance_fade) * (1.0f - centroid_distance_fade)
                                  - inverse_combined_fade * inverse_combined_fade;
    float inverse_combined_fade_sq = inverse_combined_fade * inverse_combined_fade;
    float fade_term = camera_plane_fade * distance_fade_sq_delta + inverse_combined_fade_sq;

    float planar_maximum_density = clamp01(fog->planar_maximum_density);

    global_atmospheric_fog_density = 1.0f - planar_maximum_density * fade_term;
}
