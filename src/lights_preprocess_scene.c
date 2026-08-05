/* lights_preprocess_scene @0x836F9508 — per-frame dynamic light pipeline. Retires expired/dead lights,
 * gathers the point lights visible from the current view, submits each visible light to the rasterizer
 * (computing its runtime color from its driving object function or an active flicker/transition, and its
 * radius from either the tag default or a transition-weighted blend), and submits lens flares (per-light,
 * plus any already queued from cluster-based submission earlier in the frame).
 *
 * DEVIATION: `object_try_and_get_and_verify_type`'s return value and the salted `object_header_data->data`
 * lookup both point at the *outer* per-object-type wrapper (definition_index + _object_datum + _unit_datum),
 * i.e. the same layout as the already-verified `unit_datum`/`biped_datum` types — NOT at `_object_datum`
 * directly. This was confirmed by matching two raw offsets against named DB fields only once the leading
 * 4-byte definition_index was accounted for: attached-object type/parent checks (wrapper+180/+284) land on
 * `object.type`/`object.parent_object_index`, the per-light tint lookup (wrapper+12*index+440) lands exactly
 * on `object.outgoing_change_colors[index]`, and the active-camouflage read (wrapper+892) lands exactly on
 * `unit.active_camouflage`. (Other already-committed files reading this same salted lookup with raw offsets,
 * e.g. hcex_get_light_params.c, predate this finding and may be worth revisiting.)
 *
 * DEVIATION: the decompiler renders the flags&1 check as `if (...) break;` out of the per-light `while(1)`.
 * This looked suspicious on first read (a previous pass deferred the function over it), but tracing the
 * disasm confirms the decompiler is correct: falling through (flags&1 set) lands exactly on the post-loop
 * weighted-radius code, and after an optional submit, an explicit branch rejoins the lens-flare gate that the
 * flags&1==0 path also falls into — there is no double submission, just one shared tail reached two ways.
 */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/light_datum.h"
#include "headers/lights_globals.h"
#include "headers/render_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/rasterizer_light_submit_parameters.h"
#include "headers/rasterizer_lens_flare_submit_parameters.h"
#include "headers/point_light_definition.h"
#include "headers/object_marker.h"
#include "headers/unit_datum.h"
#include "headers/cluster_partition.h"
#include "headers/real_rgb_color.h"
#include "headers/real_argb_color.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_type.h"
#include "headers/light_definition_flags.h"
#include "headers/point_light_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/datum_index.h"
#include "headers/lens_flare_window_index.h"

#define LIGHT_DATUM_SIZE 124   /* light_datum stride (includes 2-byte salt header) */


#include "headers/cluster_partition.h"
#include "headers/data_array.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_argb_color.h"
extern int game_time_get(void);
extern int data_next_index(const data_array *data, int16_t index);
extern void datum_delete(data_array *data, int index);
extern void cluster_partition_disconnect(cluster_partition *partition, int datum_index, int *first_cluster_reference);
extern void light_disconnect_from_map(uint16_t light_index);
extern void light_reconnect_to_map(uint16_t light_index);
extern void   hcex_destroy_light(int light_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

extern int16_t structure_visibility_find_objects(
    int *result_indices, int16_t maximum_count,
    int (*cluster_get_first)(int *, int16_t),
    int (*cluster_get_next)(int *),
    void (*get_bounding_sphere)(int, real_point3d *, float *),
    int (*unmarked)(uint16_t),
    int (*mark)(uint16_t));
extern int cluster_get_first_light(int *state, int16_t cluster_index);
extern int cluster_get_next_light(int *state);
extern void light_get_bounding_sphere(int light_index, real_point3d *position, float *radius);
extern int light_unmarked(uint16_t light_index);
extern int light_mark(uint16_t light_index);

extern void rasterizer_lights_begin(void);
extern void rasterizer_lens_flare_submit_for_cluster(int16_t cluster_index);
extern int rasterizer_light_submit(const rasterizer_light_submit_parameters *parameters);
extern void rasterizer_lens_flare_submit(const rasterizer_lens_flare_submit_parameters *parameters);

extern uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference);
extern int object_get_ultimate_parent(int object_index);
extern float transition_function_evaluate(int16_t function_type, float value);
extern real_rgb_color *rgb_colors_interpolate(real_rgb_color *rgb_result, unsigned int flags, const real_rgb_color *rgb_lower_bound, const real_rgb_color *rgb_upper_bound, float u);
extern real_rgb_color *rgb_colors_interpolate_and_scale(real_rgb_color *rgb_result, unsigned int flags, const real_argb_color *argb_lower_bound, const real_argb_color *argb_upper_bound, const real_rgb_color *rgb_scale, float u);
extern const char *object_get_attachment_marker_name(int object_index, int16_t attachment_index);
extern void first_person_weapon_center_flashlight(int unit_index, real_point3d *position, real_vector3d *forward, real_vector3d *up);
extern uint8_t first_person_weapon_adjust_light(int weapon_index, const char *marker_name, real_point3d *position, real_vector3d *forward, real_vector3d *up);
extern int16_t first_person_weapon_get_marker_by_name_render(int weapon_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern uint8_t compress_real_to_int8(float z);
extern unsigned int compress_real_vector3d_to_int32_clamp(const real_vector3d *v);

extern void light_marker_begin(void);
void lights_preprocess_scene(void)
{
    int current_time = game_time_get();
    debug_rasterizer_light_count = 0;

    /* Retire lights whose transition timer has expired. Every light gets its per-frame submitted-index and
     * "already submitted" flag reset regardless; a light with an active timer (parent_light_index != -1)
     * either gets reconnected to the map (still transitioning, its driving object still exists) or fully
     * deleted (transition finished). */
    for ( int light_index = data_next_index(light_data, -1); light_index != -1;
          light_index = data_next_index(light_data, light_index) )
    {
        light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, light_index);
        int transition_start_time = light->parent_light_index;

        light->rasterizer_light_index = -1;
        light->flags &= ~(1u << _point_light_attached_to_first_person_weapon_bit);

        if ( transition_start_time != -1 )
        {
            point_light_definition *definition = TAG_GET(point_light_definition, light->definition_index);
            float elapsed = (float)(current_time - transition_start_time);

            if ( elapsed <= definition->effect.duration )
            {
                if ( object_try_and_get_and_verify_type(light->object_index, object_mask_all) )
                {
                    light_disconnect_from_map(light_index);
                    light_reconnect_to_map(light_index);
                }
            }
            else
            {
                cluster_partition_disconnect(&light_cluster_partition, light_index, &light->cluster_reference);
                datum_delete(light_data, light_index);
                hcex_destroy_light(light_index);
            }
        }
    }

    /* gather the point lights visible in the current view */
    /* DEVIATION: inlined body of light_marker_begin@0x836F84D8 collapsed to call (donor takes no args, nothing to fold) */
    light_marker_begin();
    lights_globals.scene_point_light_count = structure_visibility_find_objects(
        lights_globals.scene_point_lights, 128,
        cluster_get_first_light, cluster_get_next_light,
        light_get_bounding_sphere, light_unmarked, light_mark);
    lights_globals.marker_initialized = 0;

    rasterizer_lights_begin();

    for ( int cluster = 0; cluster < render.rendered_cluster_count; ++cluster )
        rasterizer_lens_flare_submit_for_cluster(render.rendered_clusters[cluster].cluster_index);

    for ( int scene_index = 0; scene_index < lights_globals.scene_point_light_count; ++scene_index )
    {
        int light_handle = lights_globals.scene_point_lights[scene_index];
        light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, light_handle);
        point_light_definition *definition = TAG_GET(point_light_definition, light->definition_index);
        unit_datum *attached_object = object_try_and_get_and_verify_type(light->object_index, object_mask_all);

        float blend_factor;          /* 0..1 weight toward upper_bound / the transition's end state */
        float flare_alpha_scale = 1.0f;  /* dimmed below by the attached parent's active camouflage */

        if ( light->parent_light_index == -1 )
        {
            /* no active transition: drive the color from the object function, optionally tinted by a
             * per-object color-function table entry */
            float function_value;
            object_get_function_value(light->object_index, light->function_index, &function_value);

            const real_rgb_color *color_function_color;
            if ( light->color_function_index == -1 )
                color_function_color = global_real_rgb_white;
            else
                color_function_color = &((const real_rgb_color *)attached_object->object.outgoing_change_colors)
                    [light->color_function_index];

            rgb_colors_interpolate_and_scale(&light->current_color, definition->color.interpolation_flags,
                &definition->color.lower_bound, &definition->color.upper_bound,
                color_function_color, function_value);
            blend_factor = function_value;
        }
        else
        {
            float progress = (float)(current_time - light->parent_light_index) / definition->effect.duration;
            blend_factor = (1.0f - transition_function_evaluate(definition->effect.falloff_function, progress))
                * light->intensity_scale;

            rgb_colors_interpolate(&light->current_color, definition->color.interpolation_flags,
                &definition->color.lower_bound.rgb, &definition->color.upper_bound.rgb, blend_factor);
        }

        float complement = 1.0f - blend_factor;

        if ( attached_object )
        {
            int16_t ultimate_parent = object_get_ultimate_parent(light->object_index);
            unit_datum *parent = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, ultimate_parent)->datum);

            if ( (1 << parent->object.type) & object_mask_unit )   /* biped (0) or vehicle (1): the only types with unit data */
            {
                float active_camouflage = parent->unit.active_camouflage;
                if ( active_camouflage > 0.0f && (definition->flags & (1u << _light_dont_fade_active_camouflage_bit)) == 0 )
                {
                    flare_alpha_scale = 1.0f - active_camouflage;
                    light->current_color.red   *= flare_alpha_scale;
                    light->current_color.green *= flare_alpha_scale;
                    light->current_color.blue  *= flare_alpha_scale;
                }
            }
        }

        if ( light->current_color.red == 0.0f && light->current_color.green == 0.0f && light->current_color.blue == 0.0f )
            continue;   /* no visible contribution: skip both submission and lens flare */

        if ( light->flags & (1u << _point_light_dynamic_bit) )
        {
            float weighted_radius = (definition->geometry.radius_modifier_lower_bound * complement
                + definition->geometry.radius_modifier_upper_bound * blend_factor) * definition->geometry.radius;
            light->radius = weighted_radius;

            if ( weighted_radius != 0.0f )
            {
                rasterizer_light_submit_parameters submit_parameters;
                submit_parameters.definition = definition;
                submit_parameters.position = light->position;
                submit_parameters.forward = light->forward;
                submit_parameters.up = light->up;
                submit_parameters.color = light->current_color;
                submit_parameters.radius = weighted_radius;

                if ( light->parent_light_index == -1 )
                {
                    uint8_t adjusted = 0;

                    if ( definition->flags & (1u << _light_is_first_person_flashlight_bit) )
                    {
                        first_person_weapon_center_flashlight(light->object_index, &submit_parameters.position,
                            &submit_parameters.forward, &submit_parameters.up);
                        adjusted = 1;
                    }
                    else if ( attached_object && attached_object->object.type == object_type_weapon
                        && attached_object->object.parent_object_index != -1 )
                    {
                        const char *marker_name = object_get_attachment_marker_name(light->object_index,
                            light->attachment_marker_index);
                        adjusted = first_person_weapon_adjust_light(light->object_index, marker_name,
                            &submit_parameters.position, &submit_parameters.forward, &submit_parameters.up);
                    }

                    if ( adjusted )
                        light->flags |= (1u << _point_light_attached_to_first_person_weapon_bit);
                }

                light->rasterizer_light_index = rasterizer_light_submit(&submit_parameters);
                debug_rasterizer_light_count = light->rasterizer_light_index + 1;
            }
        }
        else
        {
            light->radius = definition->geometry.radius;
        }

        if ( definition->lens_flare.reference.index == -1 )
            continue;

        /* pack the flare's identity/window fields; the light-handle's low/high halves are its persistent
         * datum index / generation salt (generation -1, meaning "no salt in use", collapses to 0) */
        int16_t handle_generation = (int16_t)DATUM_INDEX_TO_IDENTIFIER(light_handle);

        rasterizer_lens_flare_submit_parameters flare_parameters;
        flare_parameters.definition = TAG_GET(struct lens_flare_definition, definition->lens_flare.reference.index);
        flare_parameters.compressed_light_color =
            ((unsigned char)(int)(flare_alpha_scale * 255.0f) << 24)
          | ((unsigned char)(int)(light->current_color.red * 255.0f) << 16)
          | ((unsigned char)(int)(light->current_color.green * 255.0f) << 8)
          |  (unsigned char)(int)(light->current_color.blue * 255.0f);
        flare_parameters.light_identifier = (int16_t)light_handle;
        flare_parameters.light_index = (handle_generation == -1) ? 0 : (uint16_t)DATUM_INDEX_TO_IDENTIFIER(light_handle);
        flare_parameters.compressed_window_index = (unsigned char)render.window_index;
        flare_parameters.compressed_light_scale = (unsigned char)compress_real_to_int8(blend_factor);

        if ( light->parent_light_index == -1 )
        {
            /* persistent light (e.g. a flashlight): resolve via the attached weapon's markers, one flare
             * copy per marker */
            const char *marker_name = object_get_attachment_marker_name(light->object_index,
                light->attachment_marker_index);
            object_marker markers[8];
            int16_t marker_count = 0;

            if ( attached_object && attached_object->object.type == object_type_weapon && attached_object->object.parent_object_index != -1 )
            {
                marker_count = first_person_weapon_get_marker_by_name_render(light->object_index, marker_name,
                    markers, 8);
                if ( marker_count > 0 )
                    flare_parameters.compressed_window_index |= LENS_FLARE_FIRST_PERSON_MARKER_FLAG;
            }

            if ( !marker_count )
                marker_count = object_get_marker_by_name(light->object_index, marker_name, markers, 8);

            /* DEVIATION: disasm shows each marker copy overwrites compressed_window_index/compressed_light_scale
             * with (marker_index, 0) rather than keeping the values set above (the |=0x80 flag included) — a
             * genuine quirk of the original code, reproduced faithfully rather than "fixed". */
            for ( int16_t marker_index = 0; marker_index < marker_count; ++marker_index )
            {
                flare_parameters.position = markers[marker_index].matrix.position;
                flare_parameters.compressed_direction = compress_real_vector3d_to_int32_clamp(&markers[marker_index].matrix.left);
                flare_parameters.compressed_up = compress_real_vector3d_to_int32_clamp(&markers[marker_index].matrix.up);
                flare_parameters.compressed_window_index = (unsigned char)marker_index;
                flare_parameters.compressed_light_scale = 0;
                rasterizer_lens_flare_submit(&flare_parameters);
            }
        }
        else
        {
            flare_parameters.position = light->position;
            flare_parameters.compressed_direction = compress_real_vector3d_to_int32_clamp(&light->forward);
            flare_parameters.compressed_up = compress_real_vector3d_to_int32_clamp(&light->up);
            flare_parameters.compressed_window_index = 0;
            flare_parameters.compressed_light_scale = 0;
            rasterizer_lens_flare_submit(&flare_parameters);
        }
    }

    for ( int16_t queued_index = 0; queued_index < lights_globals.queued_lens_flare_count; ++queued_index )
        rasterizer_lens_flare_submit(&lights_globals.queued_lens_flares[queued_index]);
    lights_globals.queued_lens_flare_count = 0;
}
