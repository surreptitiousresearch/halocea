/* _rasterizer_window_set_fog @0x8369BE10 — translate the window's resolved fog parameters into the vertex
 * shader fog constants. The incoming render_fog is copied into the global window parameters and sanitized:
 * atmospheric density defaults to 1 and is disabled (density 0, far pushed past the camera) when there is no
 * fog distance or atmospheric fog is debug-disabled; planar density defaults to 1. When planar fog is active
 * (and not flagged off / debug-disabled) the fog plane is taken from the definition, except planar_mode 2 uses
 * the camera-facing plane through the far clip; otherwise planar fog is fully disabled (white, identity plane).
 * Sixteen floats (4 vertex-shader constants: atmospheric ramp, planar plane + ramp, and the clamped density
 * blend factors) are then built and uploaded starting at constant register 6.
 *
 * DEVIATION: the decompiler split the contiguous 16-float constant block into float[12] + 4 scalars and
 * rendered the 64-bit pending mask (3 << 61) as two dwords; restored as one array and one mask. The fcfid-free
 * float math is reproduced directly. */

#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/fog_definition_flags.h"
#include "headers/render_planar_fog_mode.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/d3d_render_boundary.h"
#include "headers/real_rgb_color.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


extern void *memcpy(void *destination, const void *source, unsigned int count);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
                                                const float *pConstantData, unsigned int Vector4fCount,
                                                unsigned __int64 PendingMask0);

static float clamp_unit(float value)
{
    if (value >= 0.0f)
        return value <= 1.0f ? value : 1.0f;
    return 0.0f;
}

void _rasterizer_window_set_fog(const render_fog *fog)
{
    memcpy(&global_window_parameters.fog, fog, sizeof(global_window_parameters.fog));

    float atmospheric_maximum_density = global_window_parameters.fog.atmospheric_maximum_density;
    if (atmospheric_maximum_density <= 0.0f)
    {
        global_window_parameters.fog.atmospheric_maximum_density = 1.0f;
        atmospheric_maximum_density = 1.0f;
    }

    if (global_window_parameters.fog.atmospheric_maximum_distance == 0.0f
        || !rasterizer_debug_options.fog_atmospheric_enabled)
    {
        global_window_parameters.fog.atmospheric_minimum_distance = global_window_parameters.camera.z_far;
        atmospheric_maximum_density = 0.0f;
        global_window_parameters.fog.atmospheric_maximum_density = 0.0f;
        global_window_parameters.fog.atmospheric_maximum_distance = global_window_parameters.camera.z_far * 2.0f;
    }

    float planar_maximum_density = global_window_parameters.fog.planar_maximum_density;
    if (planar_maximum_density <= 0.0f)
    {
        global_window_parameters.fog.planar_maximum_density = 1.0f;
        planar_maximum_density = 1.0f;
    }

    const real_point3d *position = &global_window_parameters.camera.position;
    const real_vector3d *forward = &global_window_parameters.camera.forward;
    float view_dot = position->n[0] * forward->n[0]
                     + (position->n[2] * forward->n[2] + position->n[1] * forward->n[1]);
    float plane_distance;

    if (global_window_parameters.fog.planar_mode
        && (fog->fog_definition_flags & (1u << _fog_definition_screen_effect_only_bit)) == 0
        && rasterizer_debug_options.fog_planar_enabled)
    {
        if (global_window_parameters.fog.planar_mode == _render_planar_fog_mode_fully_fogged)
        {
            global_window_parameters.fog.planar_maximum_depth = 1.0f;
            global_window_parameters.fog.plane.n = global_window_parameters.camera.forward;
            plane_distance = view_dot + global_window_parameters.camera.z_far;
            global_window_parameters.fog.plane.d = plane_distance;
        }
        else
        {
            plane_distance = global_window_parameters.fog.plane.d;
        }
    }
    else
    {
        global_window_parameters.fog.planar_maximum_density = 0.0f;
        global_window_parameters.fog.planar_maximum_distance = 1.0f;
        global_window_parameters.fog.planar_maximum_depth = 1.0f;
        global_window_parameters.fog.planar_mode = 0;
        planar_maximum_density = 0.0f;
        global_window_parameters.fog.planar_color = *global_real_rgb_white;
        plane_distance = view_dot;
        global_window_parameters.fog.plane.d = plane_distance;
        global_window_parameters.fog.plane.n = global_window_parameters.camera.forward;
    }

    float inverse_planar_distance = 1.0f / global_window_parameters.fog.planar_maximum_distance;
    float inverse_planar_depth = 1.0f / global_window_parameters.fog.planar_maximum_depth;
    float inverse_atmospheric_range = 1.0f / (global_window_parameters.fog.atmospheric_maximum_distance
                                              - global_window_parameters.fog.atmospheric_minimum_distance);
    const real_vector3d *plane_normal = &global_window_parameters.fog.plane.n;

    float fog_constants[16];
    fog_constants[0] = forward->n[0] * inverse_atmospheric_range;
    fog_constants[1] = forward->n[1] * inverse_atmospheric_range;
    fog_constants[2] = forward->n[2] * inverse_atmospheric_range;
    fog_constants[3] = -((global_window_parameters.fog.atmospheric_minimum_distance + view_dot)
                         * inverse_atmospheric_range);
    fog_constants[4] = -(plane_normal->n[0] * inverse_planar_depth);
    fog_constants[5] = -(plane_normal->n[1] * inverse_planar_depth);
    fog_constants[6] = -(plane_normal->n[2] * inverse_planar_depth);
    fog_constants[7] = plane_distance * inverse_planar_depth;
    fog_constants[8] = forward->n[0] * inverse_planar_distance;
    fog_constants[9] = forward->n[1] * inverse_planar_distance;
    fog_constants[10] = forward->n[2] * inverse_planar_distance;
    fog_constants[11] = -(inverse_planar_distance * view_dot);

    float plane_dot = position->n[0] * plane_normal->n[0]
                      + (position->n[2] * plane_normal->n[2] + position->n[1] * plane_normal->n[1]);

    fog_constants[12] = clamp_unit(atmospheric_maximum_density);
    fog_constants[13] = clamp_unit(-(inverse_planar_depth * (plane_dot - plane_distance)));
    fog_constants[14] = clamp_unit(planar_maximum_density);
    fog_constants[15] = 3.0f;

    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 6, fog_constants, 4, (unsigned __int64)3 << 61);
}
