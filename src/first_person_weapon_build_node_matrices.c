/* first_person_weapon_build_node_matrices @0x8369F380 — per-frame viewmodel pose builder for one local
 * player's first-person weapon: latches the previous render facing/position (for interpolation), re-derives
 * the current render facing/position/forward from the camera, then (if the player has a valid weapon)
 * rebuilds `node_orientations` from the weapon's base pose/animation, layers overlay animations for a
 * pending firing/reload pose, movement sway, turning sway, and firing push-back, optionally cross-fades
 * against the previous frame's orientations, and finally poses `node_matrices` from the result.
 *
 * The weapon definition/graph/object fields are typed via weapon_definition, animation_graph,
 * animation_graph_first_person_weapon_animations, and weapon_datum (DB-verified layouts): the
 * firing-overlay counters live in weapon.magazines[0] (state_timer/original_time/rounds_total/rounds_loaded)
 * and the overcharge amount is weapon.overcharged; the predicted-frame clamp is the first magazine
 * definition's rounds_loaded_maximum.
 * DEVIATION: the decompiler's `LODWORD(v19)=diff; HIDWORD(v19)=0x82000000;` int-to-float conversion is
 * display noise (confirmed via disasm: it's a plain `extsw`+`fcfid` of the signed __int16 difference, not a
 * real 0x82000000 high word) — reproduced as a plain float cast.
 * DEVIATION: the decompiler loses track of the `node_orientations` pointer argument in every
 * `overlay_animation_apply*`/`interpolate_node_orientations` call after the first, showing stray locals
 * (`v13`, `v14`) instead; disasm confirms the argument register (r6/r29) is set once, early, to
 * `&fpw->node_orientations` and never changes — every one of those calls targets that same buffer.
 * FAITHFUL QUIRK: the firing-overlay frame-blend gate reads `first_person_weapons[0].state` (always local
 * player 0's weapon state), not this player's — reproduced as-is; confirmed via decompiler and disasm both
 * showing the unindexed global. */

#include <stdint.h>
#include "headers/first_person_weapon.h"
#include "headers/render_globals.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/animation.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_first_person_weapon_animations.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_magazine_definition.h"
#include "headers/model.h"
#include "headers/weapon_type.h"
#include "headers/first_person_weapon_state.h"
#include "headers/first_person_weapon_animation.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_orientation.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_orientation.h"
extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void model_get_node_orientations(const model *model, real_orientation *node_orientations);
extern void animation_get_node_orientations(const model *model, const animation *animation, int16_t frame_index, real_orientation *node_orientations);
extern void overlay_animation_apply(const animation *animation, int16_t frame_index, real_orientation *node_orientations);
extern void overlay_animation_apply_scaled(const animation *animation, int16_t frame_index, float animation_scale, real_orientation *node_orientations);
extern void overlay_animation_apply_continuous_scaled(const animation *animation, float real_frame_index, float animation_scale, real_orientation *node_orientations);
extern void interpolate_node_orientations(int16_t node_count, real_orientation *original_node_orientations, real_orientation *target_node_orientations, int16_t frame_index, int16_t frame_count);
extern void animation_graph_node_matrices_from_orientations(int animation_graph_index, real_matrix4x3 *node_matrices, const real_orientation *node_orientations, const real_point3d *origin, const real_vector3d *forward, const real_vector3d *up);

void first_person_weapon_build_node_matrices(int16_t local_player_index)
{
    first_person_weapon *fpw = &first_person_weapons[local_player_index];

    if (!fpw->rendered)
    {
        euler_angles2d_from_vector3d(&fpw->render_facing, &render.camera.forward);
        fpw->render_position = render.camera.position;
    }

    fpw->last_render_facing   = fpw->render_facing;
    fpw->last_render_position = fpw->render_position;

    euler_angles2d_from_vector3d(&fpw->render_facing, &render.camera.forward);
    fpw->render_position = render.camera.position;
    fpw->render_forward   = render.camera.forward;
    fpw->rendered = 1;

    if (fpw->weapon_index != -1 && !object_try_and_get_and_verify_type(fpw->weapon_index, object_mask_weapon))
        fpw->weapon_index = -1;

    if (fpw->weapon_index == -1)
        return;

    weapon_datum *weapon_object = (weapon_datum *)DATUM_GET(object_header_data, object_header_datum,
                                                                     fpw->weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon_object->definition_index);
    animation_graph *graph = TAG_GET(animation_graph, definition->weapon.interface_definition.first_person_animations.index);

    if (graph->first_person_weapon_animations.count && graph->first_person_weapon_animations.address)
    {
        animation_graph_first_person_weapon_animations *animation_set =
            (animation_graph_first_person_weapon_animations *)graph->first_person_weapon_animations.address;

        if (fpw->state_animation.index == -1)
        {
            const model *base_model = TAG_GET(const model, definition->weapon.interface_definition.first_person_model.index);
            model_get_node_orientations(base_model, fpw->node_orientations);
        }
        else
        {
            const animation *state_anim = &((const animation *)graph->animations.address)[fpw->state_animation.index];
            animation_get_node_orientations(0, state_anim, fpw->state_animation.frame_index, fpw->node_orientations);
        }

        __int16 firing_overlay_index = (animation_set->animations.count <= _first_person_weapon_animation_ammunition)
                                      ? -1 : ((__int16 *)animation_set->animations.address)[_first_person_weapon_animation_ammunition];

        if (firing_overlay_index != -1)
        {
            const animation *firing_animation = &((const animation *)graph->animations.address)[firing_overlay_index];
            __int16 committed_frame = weapon_object->weapon.magazines[0].rounds_loaded;
            unsigned __int8 apply_overlay = 1;

            int state = first_person_weapons->state;
            if (definition->weapon.weapon_type == _weapon_type_needler && (state == _first_person_weapon_state_reload_while_empty || state == _first_person_weapon_state_reload_while_full))
            {
                /* server-predicted frame settle: ease the committed frame toward the predicted frame over
                 * the last 0.2 (1/5) of a ~44-tick window; no frame_count bounds check on this path. */
                __int16 settle_delay = (__int16)(weapon_object->weapon.magazines[0].original_time
                                                 - weapon_object->weapon.magazines[0].state_timer);
                if (settle_delay >= 44)
                {
                    float settle_fraction = (float)(__int16)(settle_delay - 44) * 0.2f;
                    if (settle_fraction > 1.0f)
                        settle_fraction = 1.0f;

                    __int16 predicted_frame = weapon_object->weapon.magazines[0].rounds_total;
                    __int16 max_frame = ((weapon_magazine_definition *)definition->weapon.magazines.address)->rounds_loaded_maximum;
                    if (predicted_frame > max_frame)
                        predicted_frame = max_frame;

                    committed_frame = (__int16)((float)(__int16)(predicted_frame - committed_frame) * settle_fraction)
                                    + committed_frame;
                }
            }
            else if (committed_frame >= firing_animation->frame_count)
            {
                apply_overlay = 0;
            }

            if (apply_overlay)
                overlay_animation_apply(firing_animation, committed_frame, fpw->node_orientations);
        }

        if (fpw->moving_animation.index != -1)
        {
            const animation *moving_animation = &((const animation *)graph->animations.address)[fpw->moving_animation.index];
            overlay_animation_apply(moving_animation, fpw->moving_animation.frame_index, fpw->node_orientations);
        }

        if (fpw->overcharged_jitter_animation.index != -1)
        {
            const animation *jitter_animation = &((const animation *)graph->animations.address)[fpw->overcharged_jitter_animation.index];
            overlay_animation_apply_continuous_scaled(jitter_animation, fpw->overcharged_jitter_animation.frame_index,
                                                      weapon_object->weapon.overcharged + 0.5f, fpw->node_orientations);
        }

        __int16 sway_overlay_index = (animation_set->animations.count <= _first_person_weapon_animation_overlays)
                                    ? -1 : ((__int16 *)animation_set->animations.address)[_first_person_weapon_animation_overlays];

        if (sway_overlay_index != -1)
        {
            const animation *sway_animation = &((const animation *)graph->animations.address)[sway_overlay_index];
            if (sway_animation->frame_count >= 9)
            {
                float position_x = fpw->position.n[0];
                if (position_x != 0.0f)
                    overlay_animation_apply_scaled(sway_animation, (position_x < 0.0f) ? 1 : 0,
                                                   (position_x < 0.0f) ? -position_x : position_x,
                                                   fpw->node_orientations);

                float position_y = fpw->position.n[1];
                if (position_y != 0.0f)
                    overlay_animation_apply_scaled(sway_animation, (position_y < 0.0f) ? 2 : 3,
                                                   (position_y < 0.0f) ? -position_y : position_y,
                                                   fpw->node_orientations);

                float turning_x = fpw->turning.n[0];
                if (turning_x != 0.0f)
                    overlay_animation_apply_scaled(sway_animation, (turning_x < 0.0f) ? 5 : 4,
                                                   (turning_x < 0.0f) ? -turning_x : turning_x,
                                                   fpw->node_orientations);

                float turning_y = fpw->turning.n[1];
                if (turning_y != 0.0f)
                    overlay_animation_apply_scaled(sway_animation, (turning_y < 0.0f) ? 6 : 7,
                                                   (turning_y < 0.0f) ? -turning_y : turning_y,
                                                   fpw->node_orientations);

                if (fpw->firing_push_back > 0.0f)
                    overlay_animation_apply_scaled(sway_animation, 8, fpw->firing_push_back, fpw->node_orientations);
            }
        }

        if (fpw->interpolation_frame_count > 0)
        {
            interpolate_node_orientations((__int16)graph->nodes.count, fpw->original_node_orientations,
                                          fpw->node_orientations, fpw->interpolation_frame_index,
                                          fpw->interpolation_frame_count);
        }
    }

    animation_graph_node_matrices_from_orientations(definition->weapon.interface_definition.first_person_animations.index, fpw->node_matrices,
                                                    fpw->node_orientations, &render.camera.position,
                                                    &render.camera.forward, &render.camera.up);
}
