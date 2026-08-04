/* render_object_list 0x836E66F4 — recursively render an object and its attachment hierarchy. Walks the sibling
 * chain (object_data + 276) and, for each object, recurses into its first child (object_data + 280). For every
 * object it builds a per-object render_model_effect (inherited from the parent, with active-camo / type-2
 * modifier overrides), submits the model to render_model — or a dimmer 0.3x-LOD shadow proxy when data->shadow —
 * renders any attached widgets, then recurses into the children passing the freshly-built effect (or NULL for
 * the shadow pass). First-person-camera objects are skipped unless the view is mirrored.
 *
 * DEVIATION (render_model prototype): as in first_person_weapon_draw / render_model, the Xbox-360 ABI gives each
 * float argument a shadow GPR slot, so the decompiler mis-ordered both render_model calls (it grew a phantom
 * region_permutation_indices in r5 and shifted lighting/centroid/model_effect by one). The true register/stack
 * mapping, recovered from the call-setup disasm (0x836E68F8-0x836E69E8), is encoded in the 13-argument calls
 * below. Both calls' four stack args sit at incoming 0x5C/0x64/0x6E/0x74 = model_effect, unique_identifier,
 * forced_shader_permutation_index, flags. Shadow call: model_effect = NULL, flags = 2 (shadow bit), lod *= 0.3.
 * Normal call: model_effect = &child_effect, flags = (data->no_planar_fog ? 4 : 0).
 *
 * DEVIATION (effect copy): the decompiler rendered the "child_effect = *parent_model_effect" 10-dword copy as a
 * pointer walk from &parent_model_effect[-1].modifier_animation.values; that base arithmetic resolves to
 * parent_model_effect[0], i.e. a plain 40-byte struct copy (reproduced as such). On the type != 2 path the
 * original re-emits the same copy (a codegen duplicate); kept for faithfulness. */

#include <stdint.h>
#include "headers/object_render_data.h"
#include "headers/object_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/_object_definition.h"
#include "headers/render_model_effect.h"
#include "headers/render_animation.h"
#include "headers/render_globals.h"
#include "headers/render_lighting.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_rgb_color.h"
#include "headers/shader.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_flags.h"
#include "headers/object_definition_flags.h"
#include "headers/object_type.h"
#include "headers/render_model_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/render_model_effect_type.h"


#include "headers/real_matrix4x3.h"
#include "headers/render_lighting.h"
#include "headers/real_point3d.h"
extern uint8_t object_is_first_person_camera(int object_index);
extern float object_get_level_of_detail_pixels(int object_index);
extern real_matrix4x3 *object_get_node_matrices(int object_index);
extern uint8_t shader_type_is_valid_for_modifier(int16_t shader_type);
extern void object_type_render_debug(int object_index);
extern void widgets_render(int object_index, const render_lighting *lighting, const render_animation *animation);
extern void render_model(int model_index, float level_of_detail_pixels, const real_matrix4x3 *node_matrices, const char *region_permutation_indices, const real_rgb_color *change_colors, const float *function_values, const render_lighting *lighting, const real_point3d *centroid, float radius, const render_model_effect *model_effect, int unique_identifier, int16_t forced_shader_permutation_index, unsigned int flags);

void render_object_list(object_render_data *data, render_model_effect *parent_model_effect, int object_index)
{
    for ( int i = object_index; i != -1; )
    {
        object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, i)->datum;

        /* Skip first-person-camera objects unless the view is mirrored, but still advance to the next sibling. */
        if ( !object_is_first_person_camera(i) || render.camera.mirrored )
        {
            render_model_effect child_effect;
            if ( !data->shadow )
            {
                child_effect = *parent_model_effect;
                if ( parent_model_effect->type == _render_model_effect_type_transparent_zbuffered )
                {
                    /* A type-2 (modifier) parent does not propagate its modifier down to its children. */
                    child_effect.type = _render_model_effect_type_none;
                    child_effect.modifier_shader = nullptr;
                    child_effect.modifier_animation.colors = nullptr;
                    child_effect.modifier_animation.values = nullptr;
                }
                else
                {
                    child_effect = *parent_model_effect;  /* DEVIATION: original re-copies here (codegen dup) */
                }
            }

            if ( (object_data->object.flags & (1u << _object_invisible_bit)) == 0 )  /* object render-enabled (not hidden) */
            {
                _object_definition *object_def = TAG_GET(_object_definition, object_data->definition_index);
                float level_of_detail_pixels = object_get_level_of_detail_pixels(i);
                if ( data->shadow )
                {
                    render_model(
                        object_def->model.index,
                        (float)(level_of_detail_pixels * 0.30000001),
                        object_get_node_matrices(i),
                        (const char *)object_data->object.region_permutations,
                        object_data->object.outgoing_change_colors,
                        object_data->object.outgoing_function_values,
                        data->lighting,
                        &object_data->object.bounding_sphere_center,
                        object_data->object.bounding_sphere_radius,
                        nullptr, i, object_data->object.forced_shader_permutation_index, (1u << _render_model_shadow_bit));
                }
                else
                {
                    /* Inherit this object type's modifier shader, if it names a valid modifier shader. */
                    int modifier_shader_index = object_def->modifier_shader.index;
                    if ( modifier_shader_index != -1 )
                    {
                        const shader *modifier_shader = TAG_GET(const shader, modifier_shader_index);
                        child_effect.modifier_shader = modifier_shader;
                        if ( shader_type_is_valid_for_modifier(modifier_shader->base.type) )
                        {
                            child_effect.modifier_animation.colors = (const real_rgb_color *)object_data->object.outgoing_change_colors;
                            child_effect.modifier_animation.values = object_data->object.outgoing_function_values;
                        }
                        else
                        {
                            child_effect.modifier_shader = nullptr;
                        }
                    }

                    /* Active camo: bipeds and vehicles with a positive camo intensity get a type-1 camo effect. */
                    if ( ((1 << object_data->object.type) & object_mask_unit) != 0
                         && ((unit_datum *)object_data)->unit.active_camouflage > 0.0 )
                    {
                        child_effect.type = _render_model_effect_type_active_camouflage;
                        child_effect.source_object_index = i;
                        child_effect.source_object_centroid.__s1.x = object_data->object.bounding_sphere_center.n[0];
                        child_effect.source_object_centroid.__s1.y = object_data->object.bounding_sphere_center.n[1];
                        child_effect.source_object_centroid.__s1.z = object_data->object.bounding_sphere_center.n[2];
                        child_effect.intensity = ((unit_datum *)object_data)->unit.active_camouflage;
                        child_effect.parameter = ((unit_datum *)object_data)->unit.active_camouflage_super_amount;
                    }

                    /* Object types flagged in their definition (bit 1) get a type-2 modifier effect. */
                    if ( (object_def->flags & (1u << _object_transparency_self_occludes_bit)) != 0 )
                    {
                        child_effect.type = _render_model_effect_type_transparent_zbuffered;
                        child_effect.source_object_index = i;
                        child_effect.source_object_centroid.__s1.x = object_data->object.bounding_sphere_center.n[0];
                        child_effect.source_object_centroid.__s1.y = object_data->object.bounding_sphere_center.n[1];
                        child_effect.source_object_centroid.__s1.z = object_data->object.bounding_sphere_center.n[2];
                    }

                    render_model(
                        object_def->model.index,
                        level_of_detail_pixels,
                        object_get_node_matrices(i),
                        (const char *)object_data->object.region_permutations,
                        object_data->object.outgoing_change_colors,
                        object_data->object.outgoing_function_values,
                        data->lighting,
                        &object_data->object.bounding_sphere_center,
                        object_data->object.bounding_sphere_radius,
                        &child_effect, i, object_data->object.forced_shader_permutation_index,
                        data->no_planar_fog ? 4 : 0);  /* (no_planar_fog == 0) - 1 & 4 */

                    if ( debug_objects && (debug_object_index == -1 || debug_object_index == i) )
                        object_type_render_debug(i);
                }
            }

            /* Attached widgets (meters, etc.) render in the normal pass only. */
            if ( !data->shadow && object_data->object.first_widget_index != -1 )
            {
                render_animation widget_animation;
                widget_animation.colors = (const real_rgb_color *)object_data->object.outgoing_change_colors;
                widget_animation.values = object_data->object.outgoing_function_values;
                widgets_render(i, data->lighting, &widget_animation);
            }

            /* Recurse into the children with the freshly-built effect (none for the shadow pass). */
            int first_child = object_data->object.first_child_object_index;
            if ( first_child != -1 )
                render_object_list(data, data->shadow ? nullptr : &child_effect, first_child);
        }

        i = object_data->object.next_object_index;  /* next sibling */
    }
}
