/* first_person_weapon_draw 0x8369EB70 — render the local player's first-person viewmodel: the weapon model
 * and, separately, the first-person hands model. For each, the cached viewmodel node pose is remapped onto the
 * target model's node layout (model_remap...), then submitted to render_model with a per-object model effect
 * (active-camo / overshield style modifier) built from the unit's state and the unit's cached render lighting.
 * The posed node matrices are copied out to the hcex bridge (hcex_fp_model_matrices1/2) and the rendered model
 * indices stashed (hcex_fp_model1/2) so the outer ws-engine can re-render the viewmodel. When there is no local
 * player the bridge model slots are cleared. The legacy render_model calls are skipped while hcex_model_rend_
 * disable is set (the hcex bridge owns the draw), but the pose remap + bridge copy still run.
 *
 * DEVIATION 1 (render_model prototype): the database prototype is wrong. The Xbox-360 ABI passes float args in
 * f1/f2 AND reserves their positional GPR slot, so the two floats (level_of_detail_pixels, radius) each shadow a
 * GPR. Reading render_model's body, the real 13-argument signature is
 *   (model_index, lod, node_matrices [r5, posed], change_colors [r6], function_values [r7], animation_values
 *    [r8], lighting [r9, memcpy'd 0x74 == sizeof render_lighting], centroid [r10, 3 floats], radius,
 *    model_effect [stack, copied as 10 dwords], unique_identifier [stack], forced_shader_permutation_index
 *    [stack], flags [stack]). The DB carried a spurious region_permutation_indices and shifted lighting/centroid/
 *   model_effect by one. The decompiler's trailing v17/v18/v19 (read from sp+8..) are bogus; the four real stack
 *   args are confirmed at incoming offsets 0x5C/0x64/0x6E/0x74 = model_effect, unique_identifier,
 *   forced_shader_permutation_index, flags(=8).
 * DEVIATION 2 (model_remap prototype): the database 5-argument prototype is wrong. The body computes
 *   out[i] = source_node_matrices[node_remapping_table[i]] over the model's node count, so the real signature is
 *   (model_index, out_node_matrices, source_node_matrices, node_remapping_table); the database's 3rd/4th params
 *   are mistyped and its 5th is never read (the caller leaves stale junk in r7), so it is dropped here.
 * DEVIATION 3: the decompiler typed the object_get_cached_render_lighting result as a centroid (real_point3d*);
 *   it is the render_lighting* passed to render_model's lighting parameter.
 * The model_effect is only partially initialized (type + intensity/parameter/centroid or just type=0); the
 * trailing modifier_animation bytes are left as uninitialized stack, faithful to the original. */

#include <stdint.h>
#include <string.h>
#include "headers/render_globals.h"
#include "headers/first_person_weapon.h"
#include "headers/render_lighting.h"
#include "headers/render_model_effect.h"
#include "headers/real_matrix4x3.h"
#include "headers/data_array.h"
#include "headers/game_globals_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/game_globals_first_person_interface.h"
#include "headers/unit_flags.h"
#include "headers/render_model_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/render_model_effect_type.h"


/* hcex bridge: per-local-player first-person model indices + posed node matrices the outer engine re-renders. */

#include "headers/real_rgb_color.h"
#include "headers/real_point3d.h"
extern int local_player_get_player_index(int16_t local_player_index);
extern render_lighting *object_get_cached_render_lighting(int object_index, float level_of_detail_pixels);
extern void model_remap_node_matrices_to_match_animation_graph(int model_index, real_matrix4x3 *model_node_matrices, const real_matrix4x3 *source_node_matrices, const int16_t *node_remapping_table);
extern void render_model(int model_index, float level_of_detail_pixels, const real_matrix4x3 *node_matrices, const char *region_permutation_indices, const real_rgb_color *change_colors, const float *function_values, const render_lighting *lighting, const real_point3d *centroid, float radius, const render_model_effect *model_effect, int unique_identifier, int16_t forced_shader_permutation_index, unsigned int flags);

void first_person_weapon_draw(void)
{
    /* attested void: the reconstruction's `result` was pure r3-threading (final r3 == memcpy's returned
     * dst); all 3 callers ignore it. Kept the internal player index as a plain local. */
    if ( render.local_player_index == -1 )
    {
        hcex_fp_model1[0] = 0;
        hcex_fp_model1[1] = 0;
        hcex_fp_model2[1] = 0;
        hcex_fp_model2[0] = 0;
        return;
    }

    first_person_weapon *fpw = &first_person_weapons[render.local_player_index];
    int player_index = local_player_get_player_index(render.local_player_index);
    if ( player_index == -1 )
        return;

    player_index = local_player_get_player_index(render.local_player_index);
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int unit_index = player->unit_index;
    if ( unit_index == -1 || !fpw->visible || fpw->unit_index == -1 || fpw->weapon_index == -1 )
        return;

    weapon_datum *weapon_object_data = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, fpw->weapon_index)->datum;
    unit_datum *unit_object_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon_object_data->definition_index);
    if ( definition->weapon.interface_definition.first_person_animations.index == -1 )
        return;

    /* Build the per-object model effect at the front of the render scratch; the posed node matrices follow at
     * byte offset 48 (64 * sizeof(real_matrix4x3) == 3328, filling the rest of the 3376-byte buffer). */
    unsigned char render_buffer[3376];
    render_model_effect *effect = (render_model_effect *)render_buffer;
    real_matrix4x3 *posed_node_matrices = (real_matrix4x3 *)&render_buffer[48];

    effect->modifier_shader = 0;
    if ( (unit_object_data->unit.flags & (1u << _unit_active_camouflaged_bit)) != 0 || unit_object_data->unit.active_camouflage > 0.0 )
    {
        effect->type = _render_model_effect_type_active_camouflage;
        effect->intensity = unit_object_data->unit.active_camouflage;
        effect->parameter = unit_object_data->unit.active_camouflage_super_amount;
        effect->source_object_index = unit_index;
        effect->source_object_centroid = render.camera.position;
    }
    else
    {
        effect->type = 0;
    }

    render_lighting *cached_lighting = nullptr;
    int render_disabled = hcex_model_rend_disable;
    game_globals_first_person_interface *hands_interface =
        (game_globals_first_person_interface *)global_game_globals->first_person_interface.address;
    if ( !hcex_model_rend_disable )
    {
        cached_lighting = object_get_cached_render_lighting(unit_index, 3.4028235e38f);
        render_disabled = hcex_model_rend_disable;
    }

    /* Weapon model: remap the viewmodel pose onto the weapon's nodes, draw it, stash the result for the bridge. */
    if ( fpw->weapon_node_remapping_table_valid )
    {
        int weapon_model_index = definition->weapon.interface_definition.first_person_model.index;
        if ( weapon_model_index != -1 )
        {
            model_remap_node_matrices_to_match_animation_graph(
                weapon_model_index, posed_node_matrices,
                fpw->node_matrices, fpw->weapon_node_remapping_table);
            if ( !render_disabled )
            {
                render_model(
                    weapon_model_index, 0.0, posed_node_matrices, nullptr,
                    weapon_object_data->object.outgoing_change_colors,
                    weapon_object_data->object.outgoing_function_values,
                    cached_lighting, &render.camera.position, 0.0,
                    effect, fpw->weapon_index, 0, (1u << _render_model_first_person_bit));
                render_disabled = hcex_model_rend_disable;
            }
            memcpy(hcex_fp_model_matrices1[render.local_player_index],
                   posed_node_matrices, sizeof(real_matrix4x3[64]));
            hcex_fp_model1[render.local_player_index] = weapon_model_index;
        }
    }

    /* Hands model: same remap/draw/stash, using the first-person interface's hands model and the unit's data. */
    if ( fpw->hands_node_remapping_table_valid )
    {
        int hands_model_index = hands_interface->hands.index;
        if ( hands_model_index != -1 )
        {
            model_remap_node_matrices_to_match_animation_graph(
                hands_model_index, posed_node_matrices,
                fpw->node_matrices, fpw->hands_node_remapping_table);
            if ( !render_disabled )
                render_model(
                    hands_model_index, 0.0, posed_node_matrices, nullptr,
                    unit_object_data->object.outgoing_change_colors,
                    unit_object_data->object.outgoing_function_values,
                    cached_lighting, &render.camera.position, 0.0,
                    effect, fpw->weapon_index, 0, (1u << _render_model_first_person_bit));
            memcpy(hcex_fp_model_matrices2[render.local_player_index],
                   posed_node_matrices, sizeof(real_matrix4x3[64]));
            hcex_fp_model2[render.local_player_index] = hands_model_index;
        }
    }
}
