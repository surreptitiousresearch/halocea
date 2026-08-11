/* actor_select_firing_position @0x837F03E0 — the core AI firing-position query. Given an actor and an
 * evaluation context (mode, target, ranges), it: resolves the actor's current target (a specific point, or a
 * prop it is aware of) and fills the context's target fields; gathers the actor's gun offsets, avoid points,
 * and friend/enemy attack vectors; walks the encounter's firing positions and builds a candidate list of the
 * ones allowed for this actor; computes each candidate's pathfinding distance to the actor (and optionally to
 * the target); sorts them; runs the post-evaluator table; and finally scores the sorted candidates to pick the
 * best. Returns the encounter firing-position index of the winner (-1 if none), copies it into
 * best_firing_position, and reports its owning actor through current_owner.
 *
 * Reconstructed from disassembly (0x837F03E0-0x837F15E8); the decompiler printed "local variable allocation
 * has failed". Deviations, all resolved from disasm:
 *   - The decompiler's OVERLAPPED 64-bit local is two independent registers packed into one pair: its high half
 *     is actor_index (already a parameter) and its low half is the actor's SECOND tag definition (the
 *     variant/state definition). Split here into actor_index and variant_def; actor_def is the FIRST
 *     tag definition. The `*(_QWORD*)&candidates[0]...` spills of that pair are register scratch with no
 *     effect and are dropped.
 *   - CORRECTION: an earlier pass read these two tag pointers as `*(char**)(TAG_INSTANCE(idx) + 0x14)` — an
 *     extra `+ 0x14` on top of the shared `TAG_INSTANCE` macro, which already lands on the
 *     `cache_file_tag_instance.base_address` slot (see headers/global_tag_instances.h and, e.g.,
 *     actor_get_weapon_definition.c's identical one-step dereference). The extra offset would have landed 8
 *     bytes into the *next* tag-table entry instead of the intended tag data. Confirmed wrong by cross-checking
 *     every fixed byte offset used below (e.g. `actor_definition+52` as `gun_offset_stand`) against the
 *     authoritative `actor_definition`/`actor_variant_definition` layouts — they only line up without the
 *     extra `+ 0x14`. Fixed here to the one-step form used everywhere else in the codebase.
 *   - The candidate array is 512 x 60-byte firing_position (disasm stride 0x3C, cap 0x200), which the
 *     decompiler mis-sized as a 2-element local (`candidates` here); the real frame is extended by 0x7800 at the
 *     second stwux. sort_indices and owner_actor_indices are likewise the full-size scratch buffers.
 *   - r23 holds the encounter pointer during candidate build, then is reloaded with actor_index at 0x114C for
 *     the remaining calls (the decompiler's encounter/actor_index split is correct). The first arg to
 *     encounter_build_...owner_actor_indices is encounter_index (the RtlCheckStack12 "return" is a stack-probe
 *     misrender). 3.4028235e38 == FLT_MAX.
 *   - actor / encounter / the two tag definitions and prop are now typed (actor_datum, encounter_definition,
 *     actor_definition, actor_variant_definition, prop_datum); the evaluation_context, firing_position,
 *     encounter's firing_positions tag_block, and path_* structures use their modeled fields throughout —
 *     no raw byte-offset casts remain. The flee-target read (`actor->state.action_data` when
 *     action == actor_action_flee) resolves to `flee_state_data.flee_prop_index` (offset 0x1C matches
 *     exactly); this is a discriminated union keyed by actor_state_data.action, so the cast is only valid
 *     for the actor_action_flee case guarded above it.
 *   - A few call arguments obscured by spilling are marked with inline DEVIATION notes. */

#include "headers/actor_action.h"
#include "headers/actor_definition_flags2_flags.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/actor_variant_definition.h"
#include "headers/encounter_definition.h"
#include "headers/flee_state_data.h"
#include "headers/firing_position.h"
#include "headers/firing_position_evaluation_context.h"
#include "headers/prop_datum.h"
#include "headers/prop_iterator.h"
#include "headers/path_state.h"
#include "headers/path_input.h"
#include "headers/collision_bsp_test_vector_result.h"
#include <stdint.h>
#include "headers/post_evaluator_table_entry.h"
#include "headers/scenario.h"
#include "headers/structure_bsp.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include <string.h>
#include <float.h>
#include "headers/actor_danger_zone_type.h"
#include "headers/actor_combat_status.h"
#include "headers/firing_point_evaluation_mode.h"
#include "headers/prop_perception_state.h"
#include "headers/ai_line_of_sight.h"
#include "headers/blam_data_globals.h"
#include "headers/actor_vehicle_driver_type.h"
#include "headers/firing_position_attack_vector_type.h"

#include "headers/real_point3d.h"
#include "headers/path_debug_storage.h"
#include "headers/structure_bsp.h"
#include "headers/real_point3d.h"
#include "headers/real_point3d.h"
#include "headers/real_point3d.h"
#include "headers/real_point3d.h"
#include "headers/real_point3d.h"
extern post_evaluator_table_entry global_post_evaluator_table[];

extern float __fsqrts(float x);
extern double __fabs(double x);
extern float normalize3d(real_vector3d *v);
extern float point_to_line_distance_squared3d(const real_point3d *point, const real_point3d *base, const real_vector3d *height);
extern void encounter_build_firing_position_owner_actor_indices(int encounter_index, int *firing_position_owner_actor_indices);
extern void unit_estimate_position(int unit_index, int16_t estimate_mode, const real_point3d *body_position, real_vector3d *desired_facing, real_vector3d *desired_gun_offset, real_point3d *estimated_position);
extern void actor_perception_find_prop_pathfinding_location(int actor_index, int prop_index);
extern void prop_iterator_new(prop_iterator *iterator, int actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern uint8_t actor_perception_friend_prop_is_attacking(int actor_index, int friend_prop_index, real_vector3d *attack_vector);
extern void unit_get_aiming_vector(int unit_index, real_vector3d *aiming_vector);
extern uint8_t actor_nearby_firing_positions(int actor_index, real_point3d *test_point, int test_surface_index, int16_t group_selection_mode);
extern void actor_clear_discarded_firing_positions(int actor_index, uint8_t clear_temporary_only);
extern uint8_t path_3d_available(structure_bsp *structure_bsp, const real_point3d *start_point, float avoidance_distance, const real_point3d *end_point, uint8_t *path_available_out, real_point3d *path_endpoint);
extern void actor_path_input_new(int actor_index, path_input *input);
extern void path_input_new(path_input *input, float pathfinding_radius, uint8_t ignore_broken_surfaces, int ignore_source_object_index);
extern void path_input_set_start(path_input *input, const real_point3d *start_point, int start_surface_index);
extern void path_input_set_search_bounds(path_input *input, float maximum_distance);
extern void path_input_set_attractor(path_input *input, const real_point3d *attractor_point, float radius, int object_index, float weight);
extern void path_state_new(const path_input *input, path_state *state, path_debug_storage *debug);
extern uint8_t path_state_find(path_state *state);
extern int path_state_estimated_distance(path_state *state, const real_point3d *end_point, int end_surface_index, float *distance_reference, float *closest_approach_to_attractor_reference, real_vector3d *estimated_direction_reference);
extern void firing_position_pre_evaluate(int actor_index, firing_position_evaluation_context *evaluation_context, int firing_position_count, firing_position *firing_positions);
extern int firing_position_compare(int index1, int index2);
extern uint8_t firing_position_forced_evaluation(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position);
extern void firing_position_compute_line_of_sight(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position);
extern uint8_t firing_position_post_evaluate(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position);
extern void qsort_4byte(int *base, unsigned int num, int (*compare)(int, int));
extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);

int16_t actor_select_firing_position(int actor_index, firing_position_evaluation_context *evaluation_context,
                                 firing_position *best_firing_position, int *current_owner,
                                 path_state *area_path_state, uint8_t *area_path_state_valid)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    double best_evaluation = 0.0;
    int best_index = -1;

    int encounter_index = actor->meta.encounter_index;
    if ( encounter_index == -1 )
        return -1;

    encounter_definition *encounter =
        &((encounter_definition *)global_scenario->ai_encounters.address)[(uint16_t)encounter_index];
    actor_definition *actor_def = TAG_GET(actor_definition, actor->meta.definition_index);
    actor_variant_definition *variant_def = TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);

    firing_position candidates[512];
    int sort_indices[512];
    int owner_actor_indices[526];
    int candidate_count = 0;
    unsigned char discarded_overflow = 0;
    unsigned char any_in_range = 0;

    encounter_build_firing_position_owner_actor_indices(encounter_index, owner_actor_indices);

    int16_t self_owner_slot = actor->firing_positions.current_position_index;
    if ( self_owner_slot != -1 )
        owner_actor_indices[self_owner_slot] = -1;

    float allowable_range = actor->input.vehicle_driver_type ? 80.0f : 15.0f;
    float maximum_search_range = evaluation_context->maximum_search_range;
    evaluation_context->maximum_allowable_range = allowable_range;
    if ( maximum_search_range == 0.0f )
        evaluation_context->maximum_search_range = allowable_range;

    unsigned char specific_target_enable = evaluation_context->specific_target_enable;
    int16_t evaluation_mode = evaluation_context->evaluation_mode;
    evaluation_context->has_target = 0;
    evaluation_context->find_path_distance_to_target = (evaluation_mode == _firing_point_evaluation_mode_pursue);

    if ( specific_target_enable )
    {
        float target_x = evaluation_context->specific_target_point.n[0];
        float target_y = evaluation_context->specific_target_point.n[1];
        float target_z = evaluation_context->specific_target_point.n[2];
        int target_surface = evaluation_context->specific_target_surface_index;
        int16_t target_cluster = evaluation_context->specific_target_cluster_index;
        evaluation_context->target_point.n[0] = target_x;
        evaluation_context->target_point.n[1] = target_y;
        evaluation_context->target_point.n[2] = target_z;
        evaluation_context->has_target = 1;
        evaluation_context->target_pathfinding_point.n[0] = target_x;
        evaluation_context->target_pathfinding_point.n[1] = target_y;
        evaluation_context->target_pathfinding_point.n[2] = target_z;
        evaluation_context->target_pathfinding_surface_index = target_surface;
        evaluation_context->target_cluster_index = target_cluster;
        float dx = (evaluation_context->target_point.n[0] - actor->input.position.body_position.x);
        float dy = (evaluation_context->target_point.n[1] - actor->input.position.body_position.y);
        float dz = (evaluation_context->target_point.n[2] - actor->input.position.body_position.z);
        evaluation_context->target_danger_radius = 0.0f;
        evaluation_context->target_prop_index = -1;
        evaluation_context->target_vehicle_index = -1;
        evaluation_context->target_current_distance = __fsqrts(((dx * dx)
                + ((dz * dz) + (dy * dy))));
        unit_estimate_position(actor->meta.unit_index, 1, &evaluation_context->target_point, nullptr, nullptr,
                               &evaluation_context->target_head_position);
        evaluation_context->target_line_of_sight_position.n[0] = evaluation_context->target_head_position.n[0];
        evaluation_context->target_line_of_sight_position.n[1] = evaluation_context->target_head_position.n[1];
        evaluation_context->target_line_of_sight_position.n[2] = evaluation_context->target_head_position.n[2];
    }
    else
    {
        int prop_index;
        if ( (actor->state.action == actor_action_flee
              && (prop_index = actor->state.action_data.___u0.flee.flee_prop_index, prop_index != -1))
             || (prop_index = actor->target.target_prop_index, prop_index != -1) )
        {
            prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
            if ( evaluation_mode == _firing_point_evaluation_mode_pursue )
            {
                int prop_state = prop->state;
                if ( prop_state >= _prop_state_becoming_unacknowledged && prop_state <= _prop_state_acknowledged )
                    actor_perception_find_prop_pathfinding_location(actor_index, prop_index);
            }
            evaluation_context->has_target = 1;
            evaluation_context->target_point.n[0] = prop->body_position.n[0];
            evaluation_context->target_point.n[1] = prop->body_position.n[1];
            evaluation_context->target_point.n[2] = prop->body_position.n[2];
            evaluation_context->target_pathfinding_point.n[0] = prop->pathfinding_point.n[0];
            unsigned char use_last_visible = evaluation_context->use_last_visible_target_position;
            evaluation_context->target_pathfinding_point.n[1] = prop->pathfinding_point.n[1];
            evaluation_context->target_pathfinding_point.n[2] = prop->pathfinding_point.n[2];
            evaluation_context->target_pathfinding_surface_index = prop->pathfinding_surface_index;
            evaluation_context->target_cluster_index = prop->body_location.cluster_index;
            evaluation_context->target_prop_index = prop_index;
            evaluation_context->target_current_distance = prop->distance;
            evaluation_context->target_head_position.n[0] = prop->head_position.n[0];
            evaluation_context->target_head_position.n[1] = prop->head_position.n[1];
            evaluation_context->target_head_position.n[2] = prop->head_position.n[2];
            evaluation_context->target_vehicle_index = prop->vehicle_index;
            /* ground-truth field name is suicide_radius; used here as the target's danger/avoidance radius */
            evaluation_context->target_danger_radius = prop->suicide_radius;
            if ( !use_last_visible || prop->last_visible_time == -1 )
            {
                evaluation_context->target_line_of_sight_position.n[0] = prop->head_position.n[0];
                evaluation_context->target_line_of_sight_position.n[1] = prop->head_position.n[1];
                evaluation_context->target_line_of_sight_position.n[2] = prop->head_position.n[2];
            }
            else
            {
                evaluation_context->target_line_of_sight_position.n[0] = prop->last_visible_head_position.n[0];
                evaluation_context->target_line_of_sight_position.n[1] = prop->last_visible_head_position.n[1];
                evaluation_context->target_line_of_sight_position.n[2] = prop->last_visible_head_position.n[2];
            }
            int prop_state = prop->state;
            if ( prop_state >= _prop_state_uninspected_orphan && prop_state <= _prop_state_inspected_orphan )
            {
                evaluation_context->target_has_hint_vector = 1;
                evaluation_context->target_hint_vector.n[0] = prop->orphan_hint_vector.n[0];
                evaluation_context->target_hint_vector.n[1] = prop->orphan_hint_vector.n[1];
                evaluation_context->target_hint_vector.n[2] = prop->orphan_hint_vector.n[2];
            }
            evaluation_context->target_line_of_sight_optional =
                    (evaluation_context->evaluation_mode == _firing_point_evaluation_mode_guard || evaluation_context->evaluation_mode == _firing_point_evaluation_mode_avoid) ? 1 : 0;
        }
    }

    /* gun offset (stand): prefer the variant definition, fall back to the base definition */
    real_vector3d *variant_gun_stand = &variant_def->ranged_combat.gun_offset_stand;
    real_vector3d *base_gun_stand = &actor_def->perception.gun_offset_stand;
    if ( ((variant_gun_stand->n[2] * variant_gun_stand->n[2])
                 + ((variant_gun_stand->n[0] * variant_gun_stand->n[0])
                           + (variant_gun_stand->n[1] * variant_gun_stand->n[1]))) <= 0.000099999997f )
    {
        if ( ((base_gun_stand->n[2] * base_gun_stand->n[2])
                     + ((base_gun_stand->n[0] * base_gun_stand->n[0])
                               + (base_gun_stand->n[1] * base_gun_stand->n[1]))) <= 0.000099999997f )
        {
            evaluation_context->has_gun_offset_stand = 0;
        }
        else
        {
            evaluation_context->has_gun_offset_stand = 1;
            evaluation_context->gun_offset_stand = *base_gun_stand;
        }
    }
    else
    {
        evaluation_context->has_gun_offset_stand = 1;
        evaluation_context->gun_offset_stand = *variant_gun_stand;
    }

    /* gun offset (crouch): same variant-then-base fallback */
    real_vector3d *variant_gun_crouch = &variant_def->ranged_combat.gun_offset_crouch;
    real_vector3d *base_gun_crouch = &actor_def->perception.gun_offset_crouch;
    if ( ((variant_gun_crouch->n[2] * variant_gun_crouch->n[2])
                 + ((variant_gun_crouch->n[0] * variant_gun_crouch->n[0])
                           + (variant_gun_crouch->n[1] * variant_gun_crouch->n[1]))) <= 0.000099999997f )
    {
        if ( ((base_gun_crouch->n[2] * base_gun_crouch->n[2])
                     + ((base_gun_crouch->n[0] * base_gun_crouch->n[0])
                               + (base_gun_crouch->n[1] * base_gun_crouch->n[1]))) <= 0.000099999997f )
        {
            evaluation_context->has_gun_offset_crouch = 0;
        }
        else
        {
            evaluation_context->has_gun_offset_crouch = 1;
            evaluation_context->gun_offset_crouch = *base_gun_crouch;
        }
    }
    else
    {
        evaluation_context->has_gun_offset_crouch = 1;
        evaluation_context->gun_offset_crouch = *variant_gun_crouch;
    }

    if ( actor->danger_zone.danger_type > actor_danger_zone_none && actor->danger_zone.noticed_danger
         && actor->danger_zone.current_distance_from_actor < (double)(actor->danger_zone.bounding_sphere_radius + 3.0f) )
    {
        evaluation_context->find_path_direction_from_actor = 1;
    }
    evaluation_context->flying = actor->state.flying;
    if ( actor->input.vehicle_driver_type == _actor_vehicle_driver_directional_flying )
    {
        evaluation_context->directional_driving = 1;
        evaluation_context->directional_driving_cannot_stop = 1;
    }

    evaluation_context->avoid_point_count = 0;
    if ( actor->firing_positions.last_discarded_firing_position_valid )
    {
        evaluation_context->avoid_point[0].point.n[0] = actor->firing_positions.last_discarded_firing_position.x;
        evaluation_context->avoid_point[0].point.n[1] = actor->firing_positions.last_discarded_firing_position.y;
        evaluation_context->avoid_point[0].point.n[2] = actor->firing_positions.last_discarded_firing_position.z;
        evaluation_context->avoid_point[evaluation_context->avoid_point_count++].radius =
                actor_def->firing_position.old_avoidance_radius;
    }

    if ( actor_def->firing_position.friend_avoidance_radius > 0.0f )
    {
        int16_t mode = evaluation_context->evaluation_mode;
        if ( !mode || (uint16_t)mode == _firing_point_evaluation_mode_uncover || (uint16_t)mode == _firing_point_evaluation_mode_avoid )
        {
            prop_iterator avoid_iterator;
            prop_iterator_new(&avoid_iterator, actor_index);
            while ( evaluation_context->avoid_point_count < 32 )
            {
                prop_datum *prop = prop_iterator_next(&avoid_iterator);
                if ( !prop )
                    break;
                int prop_state = prop->state;
                if ( prop_state >= _prop_state_becoming_unacknowledged && prop_state <= _prop_state_acknowledged && !prop->enemy && !prop->dead && !prop->player )
                {
                    int slot = evaluation_context->avoid_point_count;
                    evaluation_context->avoid_point[slot].point.n[0] = prop->body_position.n[0];
                    evaluation_context->avoid_point[slot].point.n[1] = prop->body_position.n[1];
                    evaluation_context->avoid_point[slot].point.n[2] = prop->body_position.n[2];
                    evaluation_context->avoid_point[slot].radius = actor_def->firing_position.friend_avoidance_radius;
                    evaluation_context->avoid_point_count = slot + 1;
                }
            }
        }
    }

    evaluation_context->attack_vector_count = 0;
    evaluation_context->friend_attack_vector_count = 0;
    evaluation_context->dangerous_enemy_attack_vector_count = 0;
    unsigned char build_attack_vectors = 0;
    if ( (int)actor_def->flags < 0 && actor->state.combat_status >= _actor_combat_status_definite )
        build_attack_vectors = actor->situation.known_enemies > 0;
    if ( (actor_def->flags2 & (1u << _actor_definition_flags2_avoid_all_enemy_attack_vectors_bit)) != 0 && actor->state.combat_status >= _actor_combat_status_definite )
        build_attack_vectors = 1;
    unsigned int mode = (uint16_t)evaluation_context->evaluation_mode;
    if ( mode > _firing_point_evaluation_mode_avoid
         || (evaluation_context->evaluation_mode
             && (mode == _firing_point_evaluation_mode_panic
                 || (mode != _firing_point_evaluation_mode_cover && mode != _firing_point_evaluation_mode_uncover
                     && (mode == _firing_point_evaluation_mode_guard || mode == _firing_point_evaluation_mode_pursue)))) )
        build_attack_vectors = 0;

    if ( build_attack_vectors )
    {
        prop_iterator attack_iterator;
        prop_iterator_new(&attack_iterator, actor_index);
        while ( evaluation_context->attack_vector_count < 32 )
        {
            prop_datum *prop = prop_iterator_next(&attack_iterator);
            if ( !prop )
                break;
            int prop_state = prop->state;
            if ( prop_state >= _prop_state_becoming_unacknowledged && prop_state <= _prop_state_acknowledged && !prop->dead )
            {
                if ( !prop->enemy && (prop->player || prop->vehicle_index == -1) )
                {
                    real_vector3d friend_attack_direction;
                    if ( actor_perception_friend_prop_is_attacking(actor_index, attack_iterator.index,
                                                                   &friend_attack_direction) )
                    {
                        int slot = evaluation_context->attack_vector_count;
                        evaluation_context->attack_vectors[slot].type = (prop->player != 0);
                        evaluation_context->attack_vectors[slot].point = prop->body_position;
                        evaluation_context->attack_vectors[slot].vector = friend_attack_direction;
                        ++evaluation_context->attack_vector_count;
                        ++evaluation_context->friend_attack_vector_count;
                    }
                }
                if ( prop->enemy )
                {
                    int dangerous = actor_def->flags2 & (1u << _actor_definition_flags2_avoid_all_enemy_attack_vectors_bit);
                    if ( prop->unopposable_enemy && (prop->player || prop->shooting) )
                        dangerous = 1;
                    if ( dangerous )
                    {
                        int unit_index = prop->vehicle_index;
                        if ( unit_index == -1 )
                            unit_index = prop->unit_index;
                        int slot = evaluation_context->attack_vector_count;
                        evaluation_context->attack_vectors[slot].type = _firing_position_attack_vector_dangerous_enemy;
                        evaluation_context->attack_vectors[slot].point = prop->body_position;
                        unit_get_aiming_vector(unit_index, &evaluation_context->attack_vectors[slot].vector);
                        ++evaluation_context->attack_vector_count;
                        ++evaluation_context->dangerous_enemy_attack_vector_count;
                    }
                }
            }
        }
    }

    /* walk the encounter's firing positions and build the candidate list */
    *area_path_state_valid = 0;
    int firing_position_count = encounter->firing_positions.count;
    if ( firing_position_count > 0 )
    {
        int index = 0;
        do
        {
            firing_position_definition *positions = (firing_position_definition *)encounter->firing_positions.address;
            firing_position_definition *position = &positions[index];
            unsigned int area_bit = 1u << position->group_index;
            if ( (area_bit & evaluation_context->allowed_position_mask) != 0
                 && (evaluation_context->flying || position->surface_index != -1)
                 && (evaluation_context->evaluation_mode != _firing_point_evaluation_mode_pursue
                     || actor_nearby_firing_positions(actor_index, &position->position, position->surface_index, 1)) )
            {
                int owner = owner_actor_indices[index];
                unsigned char accept = (owner == -1);
                if ( !accept )
                {
                    if ( evaluation_context->evaluation_mode == _firing_point_evaluation_mode_guard
                         && evaluation_context->avoid_point_count < 32 )
                    {
                        int slot = evaluation_context->avoid_point_count;
                        evaluation_context->avoid_point[slot].point.n[0] = position->position.n[0];
                        evaluation_context->avoid_point[slot].point.n[1] = position->position.n[1];
                        evaluation_context->avoid_point[slot].point.n[2] = position->position.n[2];
                        evaluation_context->avoid_point[evaluation_context->avoid_point_count].radius = 4.0f;
                        ++evaluation_context->avoid_point_count;
                    }
                    actor_datum *owner_actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, owner);
                    float ox = (position->position.n[0] - owner_actor->input.position.body_position.x);
                    float oy = (position->position.n[1] - owner_actor->input.position.body_position.y);
                    float oz = (position->position.n[2] - owner_actor->input.position.body_position.z);
                    float owner_distance = __fsqrts(((ox * ox)
                            + ((oy * oy) + (oz * oz))));
                    if ( owner_distance >= 1.0f )
                    {
                        float sx = (position->position.n[0] - actor->input.position.body_position.x);
                        float sy = (position->position.n[1] - actor->input.position.body_position.y);
                        float sz = (position->position.n[2] - actor->input.position.body_position.z);
                        float self_distance = __fsqrts(((sx * sx)
                                + ((sy * sy) + (sz * sz))));
                        accept = (owner_distance >= (self_distance * 2.0f));
                    }
                }

                if ( accept )
                {
                    if ( candidate_count >= 512 )
                    {
                        if ( !discarded_overflow )
                            discarded_overflow = 1;
                    }
                    else
                    {
                        firing_position *candidate = &candidates[candidate_count++];
                        candidate->path_distance_from_actor = FLT_MAX;
                        candidate->definition = position;
                        candidate->original_index = index;
                        candidate->line_of_sight = _ai_line_of_sight_clear;
                        candidate->path_direction_from_actor = *global_zero_vector3d;
                        candidate->path_distance_to_target = FLT_MAX;
                        candidate->path_closest_approach_to_target = FLT_MAX;
                        candidate->path_direction_from_target = *global_zero_vector3d;
                        candidate->linear_distance_squared_to_target = 0.0f;
                        candidate->pre_evaluation = 0.0f;
                        candidate->evaluation = 0.0f;
                        candidate->valid = 1;
                        candidate->rejected = 0;
                    }
                }
            }
            index = (int16_t)(index + 1);
        }
        while ( index < firing_position_count );
    }

    if ( !candidate_count )
    {
        actor_clear_discarded_firing_positions(actor_index, 0);
        goto select_default;
    }

    /* path distance to target */
    if ( evaluation_context->has_target && evaluation_context->find_path_distance_to_target )
    {
        if ( evaluation_context->flying )
        {
            for ( int i = 0; i < candidate_count; i = (int16_t)(i + 1) )
            {
                firing_position_definition *definition = candidates[i].definition;
                real_vector3d to_target;
                to_target.n[0] = (definition->position.n[0] - evaluation_context->target_point.n[0]);
                to_target.n[1] = (definition->position.n[1] - evaluation_context->target_point.n[1]);
                to_target.n[2] = (definition->position.n[2] - evaluation_context->target_point.n[2]);
                if ( ((to_target.n[0] * to_target.n[0])
                             + ((to_target.n[1] * to_target.n[1])
                                       + (to_target.n[2] * to_target.n[2]))) < 400.0f )
                {
                    /* DEVIATION: path_3d_available's end_point arg is obscured by spilling; the
                     * check is target->firing-position reachability with 0 avoidance. */
                    if ( path_3d_available(global_structure_bsp, &evaluation_context->target_point, 0.0f,
                                           &definition->position,
                                           nullptr, nullptr) )
                    {
                        candidates[i].path_distance_to_target = normalize3d(&to_target);
                        if ( evaluation_context->find_path_direction_from_target )
                        {
                            candidates[i].path_direction_from_target.n[0] = to_target.n[0];
                            candidates[i].path_direction_from_target.n[1] = to_target.n[1];
                            candidates[i].path_direction_from_target.n[2] = to_target.n[2];
                        }
                    }
                }
            }
        }
        else if ( evaluation_context->target_pathfinding_surface_index != -1 )
        {
            path_input target_input;
            path_input_new(&target_input, actor_def->moving.pathfinding_radius, 0, actor->emotions.ignorant_of_broken_surfaces);
            path_input_set_start(&target_input, &evaluation_context->target_pathfinding_point,
                                 evaluation_context->target_pathfinding_surface_index);
            path_input_set_search_bounds(&target_input, 20.0f);
            path_state target_path;
            path_state_new(&target_input, &target_path, nullptr);
            path_state_find(&target_path);
            for ( int i = 0; i < candidate_count; i = (int16_t)(i + 1) )
            {
                real_vector3d *direction = &candidates[i].path_direction_from_target;
                if ( !evaluation_context->find_path_direction_from_target )
                    direction = nullptr;
                path_state_estimated_distance(&target_path, &candidates[i].definition->position,
                                              candidates[i].definition->surface_index,
                                              &candidates[i].path_distance_to_target, nullptr, direction);
            }
        }
    }

    /* path distance from the actor (or attractor-biased area path) */
    path_state *actor_path_state = area_path_state;
    if ( !evaluation_context->flying )
    {
        path_input actor_input;
        actor_path_input_new(actor_index, &actor_input);
        path_input_set_search_bounds(&actor_input, evaluation_context->maximum_search_range);
        if ( evaluation_context->attractor_enable && evaluation_context->has_target )
        {
            path_input_set_attractor(&actor_input, &evaluation_context->target_point,
                                     evaluation_context->attractor_radius, 0,
                                     evaluation_context->attractor_weight);
        }
        else if ( actor->danger_zone.danger_type > actor_danger_zone_none && (actor_def->flags2 & (1u << _actor_definition_flags2_pathfinding_ignores_danger_bit)) == 0 )
        {
            path_input_set_attractor(&actor_input, &actor->danger_zone.position,
                                     actor->danger_zone.danger_radius, 0, 10.0f);
        }
        else
        {
            path_state_new(&actor_input, area_path_state, nullptr);
            if ( path_state_find(area_path_state) )
                *area_path_state_valid = 1;
            goto have_actor_path;
        }
        path_state_new(&actor_input, area_path_state, nullptr);
        if ( path_state_find(area_path_state) )
            *area_path_state_valid = 1;
    }
have_actor_path:
    actor_path_state = area_path_state;

    if ( candidate_count > 0 )
    {
        for ( int i = 0; i < candidate_count; i = (int16_t)(i + 1) )
        {
            firing_position *candidate = &candidates[i];
            if ( evaluation_context->has_target )
            {
                float tx = (candidate->definition->position.n[0] - evaluation_context->target_point.n[0]);
                float ty = (candidate->definition->position.n[1] - evaluation_context->target_point.n[1]);
                float tz = (candidate->definition->position.n[2] - evaluation_context->target_point.n[2]);
                candidate->linear_distance_squared_to_target = ((tx * tx)
                        + ((ty * ty) + (tz * tz)));
            }
            firing_position_definition *definition = candidate->definition;
            float range_squared = (evaluation_context->maximum_search_range
                    * evaluation_context->maximum_search_range);
            real_vector3d from_actor;
            from_actor.n[0] = (definition->position.n[0] - actor->input.position.body_position.x);
            from_actor.n[1] = (definition->position.n[1] - actor->input.position.body_position.y);
            from_actor.n[2] = (definition->position.n[2] - actor->input.position.body_position.z);
            if ( ((from_actor.n[0] * from_actor.n[0])
                         + ((from_actor.n[2] * from_actor.n[2])
                                   + (from_actor.n[1] * from_actor.n[1]))) < range_squared )
            {
                if ( evaluation_context->flying )
                {
                    candidate->path_closest_approach_to_target = __fsqrts(point_to_line_distance_squared3d(
                            &evaluation_context->target_point, &actor->input.position.body_position, &from_actor));
                    float distance = __fsqrts(((from_actor.n[0] * from_actor.n[0])
                            + ((from_actor.n[1] * from_actor.n[1])
                                      + (from_actor.n[2] * from_actor.n[2]))));
                    if ( __fabs(distance) < 0.000099999997f )
                    {
                        distance = 0.0f;
                    }
                    else
                    {
                        from_actor.n[0] = ((1.0f / distance) * from_actor.n[0]);
                        from_actor.n[1] = (from_actor.n[1] * (1.0f / distance));
                        from_actor.n[2] = (from_actor.n[2] * (1.0f / distance));
                    }
                    candidate->path_distance_from_actor = distance;
                    if ( evaluation_context->find_path_direction_from_actor )
                    {
                        candidate->path_direction_from_actor.n[0] = from_actor.n[0];
                        candidate->path_direction_from_actor.n[1] = from_actor.n[1];
                        candidate->path_direction_from_actor.n[2] = from_actor.n[2];
                    }
                }
                else
                {
                    real_vector3d *direction = &candidate->path_direction_from_actor;
                    if ( !evaluation_context->find_path_direction_from_actor )
                        direction = nullptr;
                    path_state_estimated_distance(actor_path_state, &candidate->definition->position,
                                                  candidate->definition->surface_index,
                                                  &candidate->path_distance_from_actor,
                                                  &candidate->path_closest_approach_to_target, direction);
                }
            }
            if ( candidate->path_distance_from_actor >= (double)evaluation_context->maximum_search_range )
                candidate->valid = 0;
            else
                any_in_range = 1;
        }
    }

    if ( evaluation_context->allow_outside_range && !any_in_range )
    {
        unsigned int *seed = get_global_random_seed_address();
        int chosen = seed_random_range(seed, 0, candidate_count);
        *area_path_state_valid = 0;
        actor_clear_discarded_firing_positions(actor_index, 0);
        best_index = chosen;
        if ( !firing_position_forced_evaluation(actor_index, evaluation_context,
                                                &candidates[(int16_t)best_index]) )
            best_index = -1;
        goto select_result;
    }

    firing_position_pre_evaluate(actor_index, evaluation_context, candidate_count, candidates);
    if ( candidate_count > 0 )
    {
        int i = 0;
        do
        {
            sort_indices[i] = i;
            i = (int16_t)(i + 1);
        }
        while ( i < candidate_count );
    }
    global_temporary_sort_firing_position_count = candidate_count;
    global_temporary_sort_firing_position_array = candidates;
    qsort_4byte(sort_indices, candidate_count, firing_position_compare);

    evaluation_context->post_evaluation_bound = 0.0f;
    unsigned char post_bounded = 1;
    for ( int e = 0; global_post_evaluator_table[e].evaluation_function; ++e )
    {
        if ( ((1 << evaluation_context->evaluation_mode)
              & global_post_evaluator_table[e].evaluation_mode_mask) != 0 )
            post_bounded = global_post_evaluator_table[e].evaluation_function(actor_index, evaluation_context,
                                                                             nullptr);
        if ( !post_bounded )
            break;
    }
    evaluation_context->post_evaluation_bounded = post_bounded;

    if ( candidate_count > 0 )
    {
        for ( int i = 0; i < candidate_count; i = (int16_t)(i + 1) )
        {
            firing_position *candidate = &candidates[(int16_t)sort_indices[i]];
            if ( !candidate->valid
                 || (evaluation_context->post_evaluation_bounded
                     && (candidate->evaluation + evaluation_context->post_evaluation_bound) <= best_evaluation) )
            {
                break;
            }
            if ( evaluation_context->has_target )
                firing_position_compute_line_of_sight(actor_index, evaluation_context,
                                                      &candidates[(int16_t)sort_indices[i]]);
            candidate->pre_evaluation = candidate->evaluation;
            if ( firing_position_post_evaluate(actor_index, evaluation_context,
                                               &candidates[(int16_t)sort_indices[i]])
                 && candidate->evaluation > best_evaluation )
            {
                best_index = (int16_t)sort_indices[i];
                best_evaluation = candidate->evaluation;
            }
        }
        goto select_result;
    }

select_default:
    best_index = -1;
select_result:
    if ( (int16_t)best_index == -1 )
        return best_index;
    if ( best_firing_position )
        memcpy(best_firing_position, &candidates[(int16_t)best_index], sizeof(firing_position));
    int result = (uint16_t)candidates[(int16_t)best_index].original_index;
    if ( current_owner )
        *current_owner = owner_actor_indices[(int16_t)result];
    return result;
}
