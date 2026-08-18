/* rasterizer_lens_flares_draw @0x83792A80 — draw every visible lens flare's reflection sprites for the
 * active render window, then (outside the flare widget batch) draw each flare's sun-glow overlay.
 *
 * For each submitted flare that is on-screen for this window, occluded by at least one pixel, and has a
 * nonzero light-color alpha and at least one reflection: computes the light's view-space depth and its
 * "mirror" direction (the view-forward-reflected light direction, used to place secondary reflections on
 * the opposite side of screen center), a near/far distance fade, an azimuth-based extra corona rotation
 * (atan2 of the light-to-camera delta against the view-to-world left/forward axes — the same
 * atan2(left, forward) = yaw convention used elsewhere in this codebase), and three cosine-falloff ramps
 * (facing the view forward axis, facing the light's own compressed direction, and view/light alignment)
 * selectable per reflection via its brightness_scale_function. Each reflection is then positioned along
 * the mirror axis by its `offset`, tinted (either the light's own color with a freshly rescaled alpha, or
 * a static/animated per-reflection tint), and drawn as a 3D sprite.
 *
 * Clean decompile, restructured from goto/label soup into a single filtered for-loop (every "goto LABEL_67"
 * in the original is a `continue`/loop-body-end; "goto LABEL_68" when the scan exhausts is the loop's
 * natural exit). DEVIATIONS, all disasm-verified (0x83792F74-0x837930E0): (1) the decompiler pointed
 * scalars_interpolate's destination at the same buffer as rgb_colors_interpolate's ("v46->n"); the real
 * destination (r6 = sp+var_140) is a distinct float immediately preceding it. The interpolated RGB feeds the
 * packed color (fmuls @0x83792FB4/FBC/FC0) and the interpolated alpha feeds the packed alpha byte
 * (fmuls f11,f7,f31 @0x83792FAC — interpolated_alpha * brightness); only the TINT FACTOR is untouched by the
 * interpolation — it is 1.0 when tint_color is all-zero (f30 = f25 = 1.0) and the raw static tint_color.alpha
 * otherwise (f30 reloaded from offset +0x40 past the merge point). (2) the decompiler hallucinated a 6th
 * "v52" (real_vector2d*) argument to rasterizer_widget_draw_sprite3d, splitting one FPR-shadowed register
 * across two mislabeled slots; the real call is the standard 5-argument (point, radius, scale, rotation,
 * color) form, with `scale` = &corona_radius_scale and `color` = a byte-packed ARGB built inline (traced
 * through r29): either the light's own compressed_light_color with its alpha byte replaced by a freshly
 * rescaled brightness, or (when the reflection has a nonzero tint_color) each raw tint channel multiplied
 * by the corresponding static/interpolated color channel, all four channels scaled by 255.0 and packed
 * alpha:red:green:blue high-to-low — matching compressed_light_color's own byte order. */

#include <stdint.h>
#include <math.h>

#include "headers/lens_flare_window_index.h"
#include "headers/render_widget_type.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_lens_flare_submit_parameters.h"
#include "headers/lens_flare_definition.h"
#include "headers/lens_flare_reflection.h"
#include "headers/real_vector3d.h"
#include "headers/real_vector2d.h"
#include "headers/real_point3d.h"
#include "headers/real_rgb_color.h"
#include "headers/global_frame_parameters.h"
#include "headers/rasterizer_target.h"
#include "headers/lens_flare_definition_flags.h"
#include "headers/lens_flare_reflection_flags.h"
#include "headers/math_constants.h"
#include "headers/rasterizer_stencil_mode.h"
#include "headers/blam_data_globals.h"


extern uint8_t rasterizer_supports_occlusion_test(void);
extern void rasterizer_set_frustum_z(float z_near, float z_far);
extern void rasterizer_widget_begin(int16_t type, uint16_t flags);
extern void rasterizer_widget_end(void);
extern void rasterizer_set_stencil_mode(int16_t stencil_mode);
extern void rasterizer_widget_set_stencil_mode(int16_t stencil_mode);
extern uint8_t rasterizer_widget_set_texture(int16_t stage_index, int bitmap_group_index, int16_t sequence_index);
extern void rasterizer_widget_set_tint_factor(float tint_factor);
extern void rasterizer_widget_draw_sprite3d(const real_point3d *point, float radius, const real_vector2d *scale, float rotation, unsigned int color);
extern void rasterizer_sun_glow_draw(const rasterizer_lens_flare_submit_parameters *flare);

extern uint8_t *lens_flare_occlusion_test_results_get(const rasterizer_lens_flare_submit_parameters *lens_flare_parameters);
extern real_vector3d * uncompress_int32_to_real_vector3d(real_vector3d *result, unsigned int i);
extern float uncompress_int8_to_real(uint8_t i);
extern uint8_t compress_real_to_int8(float z);
extern float lens_flare_evaluate_corona_rotation_function(int16_t function, const rasterizer_lens_flare_submit_parameters *lens_flare_parameters);
extern float periodic_function_evaluate(int16_t function_type, double time);
extern real_rgb_color *rgb_colors_interpolate(real_rgb_color *rgb_result, unsigned int flags, const real_rgb_color *rgb_lower_bound, const real_rgb_color *rgb_upper_bound, float u);
extern void scalars_interpolate(float a, float b, float t, float *result);
extern float normalize3d(real_vector3d *v);

static float clamp01(float value)
{
    if ( value < 0.0f )
        return 0.0f;
    if ( value > 1.0f )
        return 1.0f;
    return value;
}

void rasterizer_lens_flares_draw(void)
{
    if ( !rasterizer_debug_options.draw_lens_flares
      || global_window_parameters.rasterizer_target != _rasterizer_target_render_primary
      || local_lens_flare_count <= 0 )
        return;

    if ( rasterizer_supports_occlusion_test() != 1 )
        rasterizer_set_frustum_z(0.031251907f, 4096.0f);
    rasterizer_widget_begin(_widget_type_internal_sprite, 0);

    if ( local_lens_flare_count > 0 )
    {
        const float deg2rad = DEG_TO_RAD;
        const float rad2deg = RAD_TO_DEG;
        const float inv255 = 0.0039215689f;
        const float half = 0.5f;

        for ( int flare_index = 0; flare_index < local_lens_flare_count; ++flare_index )
        {
            rasterizer_lens_flare_submit_parameters *flare = &local_lens_flare_parameters[flare_index];
            unsigned char *occlusion_test_result = lens_flare_occlusion_test_results_get(flare);

            real_vector3d light_direction;
            uncompress_int32_to_real_vector3d(&light_direction, flare->compressed_direction);

            if ( LENS_FLARE_WINDOW_INDEX(flare->compressed_window_index) != global_window_parameters.window_index )
                continue;

            lens_flare_definition *definition = flare->definition;
            if ( flare->internal__occlusion_pixels <= 0
              || (flare->compressed_light_color & 0xFF000000) == 0
              || definition->reflections.count <= 0 )
                continue;

            float light_x = flare->position.n[0];
            float light_y = flare->position.n[1];
            float light_z = flare->position.n[2];

            real_vector3d view_delta;
            view_delta.n[0] = light_x - global_window_parameters.camera.position.n[0];
            view_delta.n[1] = light_y - global_window_parameters.camera.position.n[1];
            view_delta.n[2] = light_z - global_window_parameters.camera.position.n[2];

            float view_depth = view_delta.n[0] * global_window_parameters.camera.forward.n[0]
                    + (view_delta.n[2] * global_window_parameters.camera.forward.n[2]
                        + view_delta.n[1] * global_window_parameters.camera.forward.n[1]);

            /* mirror direction: the light-to-camera delta reflected across the view-forward axis, used to
             * place secondary reflections along the opposite side of screen center */
            float mirror_z = (global_window_parameters.camera.forward.n[2] * view_depth - view_delta.n[2]) * 2.0f;
            float mirror_x = (global_window_parameters.camera.forward.n[0] * view_depth - view_delta.n[0]) * 2.0f;
            float mirror_y = (global_window_parameters.camera.forward.n[1] * view_depth - view_delta.n[1]) * 2.0f;

            if ( !rasterizer_supports_occlusion_test() )
                view_depth *= half;

            unsigned int occlusion_pixels = *occlusion_test_result;
            float occlusion_fraction = (float)occlusion_pixels * inv255;

            float distance_fade;
            if ( definition->far_fade_distance <= 0.0f )
            {
                distance_fade = 1.0f;
            }
            else
            {
                distance_fade = clamp01((view_depth - definition->far_fade_distance)
                        / (definition->near_fade_distance - definition->far_fade_distance));
            }

            float light_alpha = uncompress_int8_to_real((uint8_t)(flare->compressed_light_color >> 24))
                    * occlusion_fraction * distance_fade;

            float corona_rotation = lens_flare_evaluate_corona_rotation_function(definition->corona_rotation_function,
                    flare) * definition->corona_rotation_function_scale;

            /* azimuth-based extra corona rotation: atan2(left, forward) = yaw, matching the convention used
             * elsewhere in this codebase (see euler_angles2d_from_vector3d) */
            float dot_left = view_delta.n[0] * global_window_parameters.frustum.view_to_world.left.n[0]
                    + (view_delta.n[2] * global_window_parameters.frustum.view_to_world.left.n[2]
                        + view_delta.n[1] * global_window_parameters.frustum.view_to_world.left.n[1]);
            float dot_forward = view_delta.n[0] * global_window_parameters.frustum.view_to_world.forward.n[0]
                    + (view_delta.n[2] * global_window_parameters.frustum.view_to_world.forward.n[2]
                        + view_delta.n[1] * global_window_parameters.frustum.view_to_world.forward.n[1]);
            float azimuth_rotation = (float)atan2(dot_left, dot_forward) * rad2deg;

            /* cosine-falloff ramp basis: falloff_scale/falloff_bias turn the runtime cosine cutoff/falloff
             * angles into a linear 0..1 ramp over cos(angle) */
            float falloff_scale = (definition->runtime_cosine_falloff_angle == definition->runtime_cosine_cutoff_angle)
                    ? 0.0f
                    : 1.0f / (definition->runtime_cosine_falloff_angle - definition->runtime_cosine_cutoff_angle);
            float falloff_bias = -(definition->runtime_cosine_cutoff_angle * falloff_scale);

            normalize3d(&view_delta);

            /* falloff_forward: facing the view-forward axis; falloff_toward_light: facing the light's own
             * compressed direction; falloff_view_alignment: view/light alignment (opposite sign convention) */
            float falloff_forward = clamp01(-((light_direction.n[0] * global_window_parameters.camera.forward.n[0]
                        + (light_direction.n[1] * global_window_parameters.camera.forward.n[1]
                            + light_direction.n[2] * global_window_parameters.camera.forward.n[2]))
                    * falloff_scale) - falloff_bias);
            float falloff_toward_light = clamp01(-((light_direction.n[0] * view_delta.n[0]
                        + (light_direction.n[2] * view_delta.n[2] + light_direction.n[1] * view_delta.n[1]))
                    * falloff_scale) - falloff_bias);
            float falloff_view_alignment = clamp01((view_delta.n[0] * global_window_parameters.camera.forward.n[0]
                        + (view_delta.n[2] * global_window_parameters.camera.forward.n[2]
                            + view_delta.n[1] * global_window_parameters.camera.forward.n[1]))
                    * falloff_scale + falloff_bias);
            /* falloff_scale_table[0] is left at 1.0 (brightness_scale_function == 0: no falloff) */
            float falloff_scale_table[4];
            falloff_scale_table[0] = 1.0f;
            falloff_scale_table[1] = falloff_forward;
            falloff_scale_table[2] = falloff_toward_light;
            falloff_scale_table[3] = falloff_view_alignment;

            if ( light_alpha <= 0.0f )
                continue;

            float light_scale = uncompress_int8_to_real(flare->compressed_light_scale);
            if ( definition->reflections.count <= 0 )
                continue;

            for ( int reflection_index = 0; reflection_index < definition->reflections.count; ++reflection_index )
            {
                lens_flare_reflection *reflection = (lens_flare_reflection *)definition->reflections.address
                        + reflection_index;

                float brightness = (reflection->brightness_upper_bounds - reflection->brightness_lower_bounds)
                                * light_scale
                        + reflection->brightness_lower_bounds;
                brightness *= falloff_scale_table[reflection->brightness_scale_function];
                brightness *= light_alpha;
                if ( reflection_index == 0 )
                    light_alpha = brightness;

                if ( brightness <= 0.0f )
                    continue;

                float radius = (reflection->radius_upper_bounds - reflection->radius_lower_bounds) * light_scale
                        + reflection->radius_lower_bounds;

                unsigned int color;
                float tint_factor;
                if ( reflection->tint_color.__s1.alpha == 0.0f && reflection->tint_color.__s1.rgb.__s1.red == 0.0f
                  && reflection->tint_color.__s1.rgb.__s1.green == 0.0f && reflection->tint_color.__s1.rgb.__s1.blue == 0.0f )
                {
                    /* no per-reflection tint: reuse the light's own color, refreshing only its alpha byte */
                    color = (flare->compressed_light_color & 0xFFFFFF00) | (compress_real_to_int8(brightness) & 0xFF);
                    tint_factor = 1.0f;
                }
                else
                {
                    /* tint the light's static color by an (unused; see file comment) animated color, then
                     * unconditionally fall back to the raw static tint alpha as both the packed alpha
                     * channel and the tint factor */
                    real_rgb_color static_color;
                    static_color.n[0] = reflection->tint_color.__s1.rgb.__s1.red;
                    static_color.n[1] = reflection->tint_color.__s1.rgb.__s1.green;
                    static_color.n[2] = reflection->tint_color.__s1.rgb.__s1.blue;
                    float packed_alpha = brightness; /* 0x83792F28 fmr f11,f31 */
                    if ( reflection->animation_function > 1 )
                    {
                        float animation_phase = (reflection->animation_period + (float)global_frame_parameters.game_time_sec)
                                / reflection->animation_period;
                        float animation_t = periodic_function_evaluate(reflection->animation_function, animation_phase);
                        real_rgb_color interpolated_color;
                        rgb_colors_interpolate(&interpolated_color, reflection->animation_flags & 3,
                                &reflection->animation_color_lower_bound.__s1.rgb,
                                &reflection->animation_color_upper_bound.__s1.rgb, animation_t);
                        float interpolated_alpha;
                        scalars_interpolate(reflection->animation_color_lower_bound.__s1.alpha,
                                reflection->animation_color_upper_bound.__s1.alpha, animation_t, &interpolated_alpha);
                        packed_alpha = interpolated_alpha * brightness; /* 0x83792FAC fmuls f11,f7,f31 */
                        static_color.n[0] *= interpolated_color.n[0];
                        static_color.n[1] *= interpolated_color.n[1];
                        static_color.n[2] *= interpolated_color.n[2];
                    }
                    unsigned int red_byte = (uint8_t)(int)(static_color.n[0] * 255.0f);
                    unsigned int green_byte = (uint8_t)(int)(static_color.n[1] * 255.0f);
                    unsigned int blue_byte = (uint8_t)(int)(static_color.n[2] * 255.0f);
                    unsigned int alpha_byte = (uint8_t)(int)(packed_alpha * 255.0f); /* 0x83792FE4 fmuls f8,f11,f19; f19=255.0 */
                    color = (alpha_byte << 24) | (red_byte << 16) | (green_byte << 8) | blue_byte;
                    tint_factor = reflection->tint_color.__s1.alpha;
                }

                real_vector2d corona_radius_scale;
                float rotation_offset;
                if ( reflection_index != 0 )
                {
                    rotation_offset = reflection->rotation_offset;
                    corona_radius_scale.n[0] = 1.0f;
                    corona_radius_scale.n[1] = 1.0f;
                }
                else
                {
                    rotation_offset = reflection->rotation_offset + corona_rotation;
                    corona_radius_scale = definition->corona_radius_scale;
                }
                if ( reflection->flags & (1u << _lens_flare_reflection_rotate_from_center_of_screen_bit) )
                    rotation_offset += azimuth_rotation;
                if ( reflection->flags & (1u << _lens_flare_reflection_radius_scaled_by_occlusion_bit) )
                    radius = ((occlusion_fraction + 1.0f) * radius) * half;
                if ( reflection->flags & (1u << _lens_flare_reflection_radius_not_scaled_by_distance_bit) )
                    radius *= view_depth;

                real_point3d reflection_point;
                reflection_point.n[0] = reflection->offset * mirror_x + light_x;
                reflection_point.n[1] = reflection->offset * mirror_y + light_y;
                reflection_point.n[2] = reflection->offset * mirror_z + light_z;

                if ( rasterizer_widget_set_texture(0, definition->primary_map.index, reflection->bitmap_index) )
                    break;
                rasterizer_widget_set_tint_factor(tint_factor);

                int16_t stencil_mode = _rasterizer_stencil_mode_none;
                if ( (reflection->flags & (1u << _lens_flare_reflection_zbuffer_bit)) != 0 && LENS_FLARE_IS_FIRST_PERSON_MARKER(flare->compressed_window_index) )
                    stencil_mode = _rasterizer_stencil_mode_reject;
                rasterizer_widget_set_stencil_mode(stencil_mode);

                rasterizer_widget_draw_sprite3d(&reflection_point, radius, &corona_radius_scale,
                        rotation_offset * deg2rad, color);
            }
        }
    }

    rasterizer_set_stencil_mode(0);
    if ( rasterizer_supports_occlusion_test() != 1 )
        rasterizer_set_frustum_z(0.0f, 0.0f);
    rasterizer_widget_end();

    if ( !rasterizer_globals.alpha_render_targets_disabled && rasterizer_debug_options.lens_flare_sun_glow_enabled )
    {
        for ( int flare_index = 0; flare_index < local_lens_flare_count; ++flare_index )
        {
            rasterizer_lens_flare_submit_parameters *flare = &local_lens_flare_parameters[flare_index];
            if ( flare->internal__occlusion_pixels > 0
              && LENS_FLARE_WINDOW_INDEX(flare->compressed_window_index) == global_window_parameters.window_index
              && (flare->definition->occlusion_radius == 50.0f || (flare->definition->flags & (1u << _lens_flare_sun_bit)) != 0) )
                rasterizer_sun_glow_draw(flare);
        }
    }
}
