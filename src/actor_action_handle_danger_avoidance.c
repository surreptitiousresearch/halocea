/* actor_action_handle_danger_avoidance @0x837F4E48 — the per-tick handler that makes an actor react to a nearby
 * danger (a tracked grenade / explosive line). It decides whether to dive out of the way and/or switch into the
 * generic "avoid" action.
 *
 * The danger is modelled as a line segment (base at actor+688, direction actor+712 - actor+688) with a radius
 * (actor+660). The handler:
 *   1. Bails unless there is an active, un-suppressed danger and the actor is near its danger reference point,
 *      not already busy in a move animation.
 *   2. Measures how close the actor body / its path destination / its path re-entry are to the danger line, and
 *      discards the current firing position if the path leads back through the danger.
 *   3. If the body is inside the danger radius, computes an urgency from the vector/sphere intersection time and,
 *      by danger state (1/2/3) and the acknowledge timer, decides if the reaction is "urgent"; fires an AI
 *      communication event the first time; then tries to dive along the escape direction found by
 *      actor_action_find_escape_from_danger (with an extra ledge allowance when the actor's unit tag permits it).
 *   4. Finally, if nothing dived and the body is within the wider escape radius, switches to the avoid action
 *      (action type 13) when the current action's class permits it.
 *
 * One of the actor_action_handle_* siblings; the actor datum is read at raw byte offsets per that convention.
 *
 * DEVIATION: actor_action_try_to_dive's alignment_vector argument (decompiler local v24) is &alignment_vector
 * (the real_vector2d actor_action_find_escape_from_danger filled) — the decompiler lost the assignment; the
 * prototype pairing (find_escape's alignment_vector_reference == try_to_dive's alignment_vector) makes it exact.
 *
 * DEVIATION: actor typed as actor_datum* to name meta.unit_index (+24), meta.definition_index (+88),
 * input.vehicle_index (+344), input.position.body_position (+300), and state.action (+108, replacing a
 * redundant second actor_data->data+1828*index relookup — same slot as `actor`, one tick, immutable).
 * Also named via danger_zone (+640..+747, see actor_danger_zone.h), emotions.forced_to_charge (+885),
 * firing_positions.current_position_index (+952), control.moving (+1284), and
 * control.moving_towards_vector (+1304..+1312) — all cross-referenced against their writers and then
 * confirmed byte-for-byte against a later-supplied authoritative actor_datum member layout (see
 * actor_datum.h, including two field-name corrections that layout revealed).
 * Field names in danger_zone updated to match C:/halo/headers_ref/ authoritative dump:
 * acknowledged→noticed_danger, moved_to_owner_team→attached_to_us, source→hostility,
 * point→position, source_radius→danger_radius, predicted_point→predict_danger_position,
 * midpoint→bounding_sphere_center, computed_radius→bounding_sphere_radius,
 * hostility_decided→communicated, dive_allowed→allow_dive_evasion, timer→danger_data union.
 *
 * Byte +352 (input.vehicle_passenger) and the path endpoint point at +1196
 * (control.path.path.endpoint.point) are now accessed via typed struct fields. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_vector2d.h"
#include "headers/action_state_data.h"
#include "headers/avoid_state_data.h"
#include "headers/action_specification.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/actor_danger_zone_type.h"
#include "headers/actor_action.h"
#include "headers/action_class.h"
#include "headers/ai_communication_type.h"
#include "headers/ai_communication_hostility.h"
#include "headers/blam_data_globals.h"

struct ai_information_data;

#include "headers/ai_information_data.h"
#include "headers/avoid_state_data.h"
extern action_specification global_action_functions[];

extern uint8_t actor_move_animation_busy(uint16_t actor_index);
extern uint8_t actor_path_has_path(uint16_t actor_index);
extern uint8_t actor_path_at_destination(uint16_t actor_index);
extern float point_to_line_distance_squared3d(const real_point3d *point, const real_point3d *base, const real_vector3d *height);
extern float vector_to_line_distance_squared3d(const real_point3d *point, const real_vector3d *vector, const real_point3d *base, const real_vector3d *height);
extern float fast_vector_intersection_with_sphere(const real_point3d *point, const real_vector3d *vector, const real_point3d *center, float radius);
extern void actor_discard_firing_position(uint16_t actor_index, int16_t firing_position_index, uint8_t temporary);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern int actor_action_find_escape_from_danger(int actor_index, int16_t *escape_direction_reference, float *escape_distance_reference, real_vector2d *alignment_vector_reference, uint8_t *escape_is_ledge_reference);
extern uint8_t actor_action_try_to_dive(int actor_index, int16_t escape_direction, float dive_distance, real_vector2d *alignment_vector, float maximum_ledge_height);
extern uint8_t action_avoid_setup(int actor_index, avoid_state_data *state_data);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);

int actor_action_handle_danger_avoidance(int actor_index)
{
    int action_result = 0;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    bool within_escape_radius = false;
    bool path_re_enters_danger = false;

    uint8_t escape_is_ledge[2];
    uint16_t escape_direction[3];
    real_vector3d danger_line_delta;
    float escape_distance;
    real_vector3d path_direction_scaled;
    real_vector2d alignment_vector;
    action_state_data action_data;

    if ( !actor->danger_zone.danger_type )
        goto try_avoid_action;
    if ( !actor->danger_zone.noticed_danger )
        goto try_avoid_action;
    if ( actor->danger_zone.attached_to_us )
        goto try_avoid_action;

    const real_point3d *actor_body_position = &actor->input.position.body_position;

    {
        float reference_dx = actor->danger_zone.bounding_sphere_center.x - actor_body_position->x;
        float reference_dz = actor->danger_zone.bounding_sphere_center.z - actor_body_position->z;
        float reference_reach = actor->danger_zone.bounding_sphere_radius + 3.0f;
        float reference_dy = actor->danger_zone.bounding_sphere_center.y - actor_body_position->y;
        if ( ((reference_dy * reference_dy)
                    + ((reference_dz * reference_dz) + (reference_dx * reference_dx)))
                > (double)(reference_reach * reference_reach)
                || actor_move_animation_busy(actor_index)
                || actor->input.vehicle_passenger )
            goto try_avoid_action;
    }

    const real_point3d *danger_line_base = &actor->danger_zone.position;
    float danger_radius = actor->danger_zone.danger_radius;

    danger_line_delta.n[0] = actor->danger_zone.predict_danger_position.x - danger_line_base->x;
    danger_line_delta.n[1] = actor->danger_zone.predict_danger_position.y - danger_line_base->y;
    danger_line_delta.n[2] = actor->danger_zone.predict_danger_position.z - danger_line_base->z;

    float body_line_distance_sq =
            point_to_line_distance_squared3d(actor_body_position, danger_line_base, &danger_line_delta);
    bool body_within_danger = body_line_distance_sq < (danger_radius * danger_radius);
    within_escape_radius = body_line_distance_sq
            < ((danger_radius + 3.5f) * (danger_radius + 3.5f));

    bool destination_within_danger;
    if ( actor_path_has_path(actor_index) )
    {
        destination_within_danger = point_to_line_distance_squared3d(&actor->control.path.path.endpoint.point,
                danger_line_base, &danger_line_delta) < (double)(danger_radius * danger_radius);
        if ( actor->control.moving && !body_within_danger && !destination_within_danger )
        {
            path_direction_scaled.n[0] = actor->control.moving_towards_vector.n[0] * 3.0f;
            path_direction_scaled.n[1] = actor->control.moving_towards_vector.n[1] * 3.0f;
            path_direction_scaled.n[2] = actor->control.moving_towards_vector.n[2] * 3.0f;
            path_re_enters_danger = vector_to_line_distance_squared3d(actor_body_position, &path_direction_scaled,
                    danger_line_base, &danger_line_delta) < (double)(danger_radius * danger_radius);
        }
    }
    else
    {
        destination_within_danger = body_within_danger;
        path_re_enters_danger = false;
    }

    if ( destination_within_danger || (path_re_enters_danger && !body_within_danger) )
    {
        int firing_position = (uint16_t)actor->firing_positions.current_position_index;
        if ( firing_position != 0xFFFF )
            actor_discard_firing_position(actor_index, firing_position, 1u);
    }

    if ( !body_within_danger )
        goto try_avoid_action;

    char reaction_is_urgent = 0;
    float intersection = fast_vector_intersection_with_sphere(danger_line_base, &danger_line_delta,
            actor_body_position, danger_radius);
    float urgency = intersection;
    if ( intersection < 3.4028235e38f )
        urgency = (intersection * 45.0f);

    int danger_state = (uint16_t)actor->danger_zone.danger_type;
    bool state_reaction_urgent;
    switch ( danger_state )
    {
        case actor_danger_zone_suicide:
            if ( urgency != 0.0f )
                goto after_urgency;
            {
                int16_t acknowledge_timer = actor->danger_zone.___u19.suicide.time_until_death;
                if ( acknowledge_timer == -1 )
                    goto after_urgency;
                state_reaction_urgent = acknowledge_timer < 30;
            }
            break;
        case actor_danger_zone_projectile:
            if ( urgency != 0.0f )
                goto after_urgency;
            {
                int16_t acknowledge_timer = actor->danger_zone.___u19.projectile.time_until_explosion;
                if ( acknowledge_timer == -1 )
                    goto after_urgency;
                state_reaction_urgent = acknowledge_timer < 20;
            }
            break;
        case actor_danger_zone_vehicle:
            state_reaction_urgent = urgency < 30.0f;
            break;
        default:
            goto after_urgency;
    }
    if ( state_reaction_urgent )
        reaction_is_urgent = 1;

after_urgency:
    if ( (!actor->danger_zone.hostility || reaction_is_urgent) && !actor->danger_zone.communicated )
    {
        unsigned int danger_source = (uint16_t)actor->danger_zone.hostility;
        int16_t hostility = -1;
        if ( actor->danger_zone.hostility )
        {
            if ( danger_source == 1 )
                hostility = _comm_hostility_friend;
            else if ( danger_source < 3 )
                hostility = _comm_hostility_self;
        }
        else
        {
            hostility = _comm_hostility_enemy;
        }
        if ( (int16_t)danger_state == actor_danger_zone_projectile )
            ai_communication_event(_ai_communication_grenade_danger, actor->meta.unit_index, -1, hostility, -1, -1, nullptr);
        actor->danger_zone.communicated = 1;
    }

    uint8_t escape_found = actor_action_find_escape_from_danger(actor_index, (int16_t *)escape_direction,
            &escape_distance, &alignment_vector, escape_is_ledge);
    if ( escape_direction[0] != 0xFFFF && actor->danger_zone.allow_dive_evasion && actor->input.vehicle_index == -1 )
    {
        int dive_state = (uint16_t)actor->danger_zone.danger_type;
        char should_dive = 0;
        switch ( dive_state )
        {
            case actor_danger_zone_suicide:
                if ( (!escape_found && !escape_is_ledge[0]) || urgency != 0.0f )
                    goto check_urgent;
                break;
            case actor_danger_zone_projectile:
                if ( (!escape_found && !escape_is_ledge[0]) || urgency >= 7.0f )
                    goto check_urgent;
                break;
            case actor_danger_zone_vehicle:
check_urgent:
                if ( !reaction_is_urgent )
                    goto after_dive_decision;
                break;
            default:
                goto after_dive_decision;
        }
        should_dive = 1;
after_dive_decision:
        if ( should_dive )
        {
            float ledge_allowance;
            actor_definition *definition = TAG_GET(actor_definition, actor->meta.definition_index);
            if ( (definition->flags & (1u << _actor_definition_dive_off_ledges_bit)) != 0 )
                ledge_allowance = 8.0f;
            else
                ledge_allowance = 0.0f;
            action_result = actor_action_try_to_dive(actor_index, escape_direction[0], escape_distance,
                    &alignment_vector, ledge_allowance);
        }
    }

try_avoid_action:
    if ( (uint8_t)action_result || !within_escape_radius
            || (!actor_path_at_destination(actor_index) && actor->control.moving && !path_re_enters_danger) )
        return action_result;
    if ( actor->emotions.forced_to_charge )
        return action_result;

    int16_t action_class = global_action_functions[actor->state.action].action_class;
    if ( action_class != _action_class_passive && action_class != _action_class_pursuit )
        return action_result;
    if ( !action_avoid_setup(actor_index, &action_data.___u0.avoid) )
        return action_result;
    actor_action_change(actor_index, actor_action_avoid, &action_data);
    return 1;
}
